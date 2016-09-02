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

#include "add-contextual-data-filter-selector.h"
#include "syslog-ng.h"
#include "messages.h"
#include "filter/filter-expr.h"
#include "filter/filter-expr-parser.h"
#include "filter/filter-pipe.h"
#include <stdio.h>

typedef struct _AddContextualDataFilterSelector
{
  AddContextualDataSelector super;
  gchar *filters_path;
  GlobalConfig *cfg;
  GHashTable *filter_store;
  GList *filters_in_insertion_order;
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

static gboolean
_populate_filter_store(AddContextualDataFilterSelector *self)
{
  GList *filter_name;
  for (filter_name = self->filters_in_insertion_order; filter_name != NULL; filter_name = filter_name->next)
    {
      LogExprNode *node = cfg_tree_get_object(&self->cfg->tree, ENC_FILTER, filter_name->data);
      if (!node)
        {
          msg_error("No such filter in database", evt_tag_str("filter", filter_name->data));
          return FALSE;
        }
      LogFilterPipe *filter_pipe = (LogFilterPipe *) node->children->object;
      FilterExprNode *selected_filter = filter_expr_ref(filter_pipe->expr);
      filter_expr_init(selected_filter, self->cfg);
      g_hash_table_insert(self->filter_store, filter_name->data, selected_filter);
    }
  return TRUE;
}

static gboolean
_init(AddContextualDataSelector *s, ContextInfoDB *context_info_db)
{
  AddContextualDataFilterSelector *self = (AddContextualDataFilterSelector *)s;
  self->filters_in_insertion_order = context_info_db_ordered_selectors(context_info_db);

  if (!_init_filters_from_file(self))
      return FALSE;

  if (!_populate_filter_store(self))
      return FALSE;

  return TRUE;
}

static gchar*
_resolve(AddContextualDataSelector *s, LogMessage *msg)
{
  AddContextualDataFilterSelector *self = (AddContextualDataFilterSelector *)s;

  for (filter_name = self->filters_in_insertion_order; filter_name != NULL; filter_name = filter_name->next)
    {
      FilterExprNode *filter = g_hash_table_lookup(self->filter_store, filter_name->data);  
      if (filter != NULL && filter_expr_eval(filter, msg))
          return g_strdup(filter_name->data);
    }
  return NULL;
}

static void
_free(AddContextualDataSelector *s)
{
  AddContextualDataFilterSelector *self = (AddContextualDataFilterSelector *)s;
  cfg_free(self->cfg);
  g_hash_table_unref(self->filter_store);
  g_free(self);
}

static AddContextualDataSelector *
_clone(AddContextualDataSelector *s, GlobalConfig *cfg)
{
  AddContextualDataFilterSelector *self = (AddContextualDataFilterSelector *)s;
  AddContextualDataFilterSelector *cloned = (AddContextualDataFilterSelector *)add_contextual_data_filter_selector_new(cfg, self->filters_path);
  cloned->filter_store = g_hash_table_ref(self->filter_store);

  return &cloned->super;
}

AddContextualDataSelector*
add_contextual_data_filter_selector_new(GlobalConfig *cfg, const gchar *filters_path)
{
  AddContextualDataFilterSelector *new_instance = g_new0(AddContextualDataFilterSelector, 1);
  new_instance->super.resolve = _resolve;
  new_instance->super.free = _free;
  new_instance->super.init = _init;
  new_instance->super.clone = _clone;

  new_instance->filters_path = g_strdup(filters_path);
  new_instance->cfg = NULL;
  new_instance->filter_store = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);
  new_instance->filters_in_insertion_order = NULL;

  return &new_instance->super;
}

