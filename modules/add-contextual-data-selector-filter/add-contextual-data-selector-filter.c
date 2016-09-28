/*
 * Copyright (c) 2016 Balabit
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#include "add-contextual-data-selector-filter.h"
#include "syslog-ng.h"
#include "cfg.h"
#include "messages.h"
#include "filter/filter-expr.h"
#include "filter/filter-expr-parser.h"
#include "filter/filter-pipe.h"

typedef struct _AddContextualDataFilterSelector
{
  AddContextualDataSelector super;
  gchar *filters_path;
  GlobalConfig *cfg;
  GHashTable *filter_store;
} AddContextualDataFilterSelector;

static gboolean
_init_filters_from_file(AddContextualDataFilterSelector *self)
{
  self->cfg = cfg_new(0);
  if (!cfg_read_config(self->cfg, self->filters_path, FALSE, NULL))
    {
      cfg_free(self->cfg);
      self->cfg = NULL;
      msg_error("Error parsing filters of rule engine", evt_tag_str(EVT_TAG_FILENAME, self->filters_path));
      return FALSE;
    }
  return TRUE;
}

static FilterExprNode *
_init_filter_from_log_node(GlobalConfig *cfg, LogExprNode *node)
{
  LogFilterPipe *filter_pipe = (LogFilterPipe *) node->children->object;
  FilterExprNode *selected_filter = filter_expr_ref(filter_pipe->expr);

  filter_expr_init(selected_filter, cfg);

  return selected_filter;
}

static gboolean
_populate_filter_store(AddContextualDataFilterSelector *self)
{
  GList *objects_in_cfg = cfg_tree_get_objects(&self->cfg->tree);
  GList *cfg_object;
  for (cfg_object = objects_in_cfg; cfg_object != NULL; cfg_object = cfg_object->next)
    {
      LogExprNode *node = (LogExprNode *) cfg_object->data;
      if (node->content != ENC_FILTER)
      {
        msg_error("Error populating filter store; non-filter object in config");
        g_list_free(objects_in_cfg);
        return FALSE;
      }

      FilterExprNode *selected_filter = _init_filter_from_log_node(self->cfg, node);
      g_hash_table_insert(self->filter_store, node->name, selected_filter);
    }

  g_list_free(objects_in_cfg);
  return TRUE;
}

static gboolean
_init(AddContextualDataSelector *s)
{
  AddContextualDataFilterSelector *self = (AddContextualDataFilterSelector *)s;

  if (!_init_filters_from_file(self))
      return FALSE;

  if (!_populate_filter_store(self))
      return FALSE;

  return TRUE;
}

static GList*
_resolve(AddContextualDataSelector *s, LogMessage *msg)
{
  AddContextualDataFilterSelector *self = (AddContextualDataFilterSelector *)s;

  GList *current_filter;
  GList *matching_filters = NULL;
  GList *filter_names = g_hash_table_get_keys(self->filter_store);
  for (current_filter = filter_names; current_filter != NULL; current_filter = current_filter->next)
    {
      FilterExprNode *filter = (FilterExprNode *) g_hash_table_lookup(self->filter_store, current_filter->data);
      if (filter != NULL && filter_expr_eval(filter, msg))
          matching_filters = g_list_append(matching_filters, g_strdup(current_filter->data));
    }
  return matching_filters;
}

static void
_free(AddContextualDataSelector *s)
{
  AddContextualDataFilterSelector *self = (AddContextualDataFilterSelector *)s;
  if (self->cfg)
    cfg_free(self->cfg);
  g_hash_table_unref(self->filter_store);
  g_free(self->filters_path);
  g_free(self);
}

static AddContextualDataSelector *
_clone(AddContextualDataSelector *s, GlobalConfig *cfg)
{
  AddContextualDataFilterSelector *self = (AddContextualDataFilterSelector *)s;
  AddContextualDataFilterSelector *cloned = (AddContextualDataFilterSelector *)add_contextual_data_selector_filter_new(cfg, self->filters_path);

  return &cloned->super;
}

AddContextualDataSelector*
add_contextual_data_selector_filter_new(GlobalConfig *cfg, const gchar *filters_path)
{
  AddContextualDataFilterSelector *new_instance = g_new0(AddContextualDataFilterSelector, 1);
  new_instance->super.resolve = _resolve;
  new_instance->super.free = _free;
  new_instance->super.init = _init;
  new_instance->super.clone = _clone;

  new_instance->filters_path = g_strdup(filters_path);
  new_instance->cfg = NULL;
  new_instance->filter_store = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);

  return &new_instance->super;
}

