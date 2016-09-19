#include "add-contextual-data-template-selector.h"
#include "add-contextual-data-filter-selector.h"
#include "logmsg/logmsg.h"
#include "template/macros.h"
#include "cfg.h"
#include "apphook.h"
#include <criterion/criterion.h>
#include <unistd.h>

static LogMessage*
_create_log_msg(const gchar *message, const gchar *host)
{
  LogMessage *msg = NULL;
  msg = log_msg_new_empty();
  log_msg_set_value(msg, LM_V_MESSAGE, message, -1);
  log_msg_set_value(msg, LM_V_HOST, host, -1);

  return msg;
}

Test(add_contextual_data_template_selector, test_given_empty_selector_when_resolve_then_result_is_null)
{
  AddContextualDataSelector *selector = NULL;
  LogMessage *msg = _create_log_msg("testmsg", "localhost");

  cr_assert_null(add_contextual_data_selector_resolve(selector, msg), "When selector is NULL the resolve should return NULL.");
  log_msg_unref(msg);
}

static AddContextualDataSelector*
_create_template_selector(const gchar *template_string)
{
  GlobalConfig *cfg = cfg_new(VERSION_VALUE);
  AddContextualDataSelector *selector = add_contextual_data_template_selector_new(cfg, template_string);
  add_contextual_data_selector_init(selector, NULL);

  return selector;
}

static const gchar*
_setup_filter_cfg(const gchar *cfg_content, gint size)
{
  gchar tmp_filename[] = "testfiltersXXXXXX";
  gint fd = mkstemp(tmp_filename);
  write(fd, cfg_content, size);
  close(fd);

  return g_strdup(tmp_filename);
}

static AddContextualDataSelector*
_create_filter_selector(const gchar *filter_cfg, gint size)
{
  GlobalConfig *cfg = cfg_new(VERSION_VALUE);
  const gchar *filter_path = _setup_filter_cfg(filter_cfg, size);
  AddContextualDataSelector *selector = add_contextual_data_filter_selector_new(cfg, filter_path);
  if (!add_contextual_data_selector_init(selector))
    return NULL;

  return selector;
}

Test(add_contextual_data_template_selector, test_given_template_selector_when_resolve_then_result_is_the_formatted_template_value)
{
  AddContextualDataSelector *selector = _create_template_selector("$HOST");
  LogMessage *msg = _create_log_msg("testmsg", "localhost");
  GList *resolved_selector = add_contextual_data_selector_resolve(selector, msg);

  cr_assert_str_eq(resolved_selector->data, "localhost", "");
  g_list_free(resolved_selector);
  log_msg_unref(msg);
  add_contextual_data_selector_free(selector);
}

Test(add_contextual_data_template_selector, test_template_selector_cannot_be_resolved)
{
  AddContextualDataSelector *selector = _create_template_selector("$PROGRAM");
  LogMessage *msg = _create_log_msg("testmsg", "localhost");
  GList *resolved_selector = add_contextual_data_selector_resolve(selector, msg);

  cr_assert_str_eq(resolved_selector->data, "", "No template should be resolved.");
  log_msg_unref(msg);
  add_contextual_data_selector_free(selector);
}

Test(add_contextual_data_filter_selector, test_matching_host_filter_selection)
{
  const gchar cfg_content[] = "filter f_localhost {"\
                              "   host(\"localhost\");"\
                              "};";
  AddContextualDataSelector *selector = _create_filter_selector(cfg_content, 48);
  LogMessage *msg = _create_log_msg("testmsg", "localhost");
  GList *resolved_selector = add_contextual_data_selector_resolve(selector, msg);

  cr_assert_str_eq(resolved_selector->data, "f_localhost", "Filter name is resolved.");
  g_list_free(resolved_selector);
}

Test(add_contextual_data_filter_selector, test_matching_msg_filter_selection)
{
  const gchar cfg_content[] = "filter f_msg {"\
                               "    message(\"testmsg\");"\
                               "};";
  AddContextualDataSelector *selector = _create_filter_selector(cfg_content, 41);
  LogMessage *msg = _create_log_msg("testmsg", "localhost");
  GList *resolved_selector = add_contextual_data_selector_resolve(selector, msg);

  cr_assert_str_eq(resolved_selector->data, "f_msg", "Filter name is resolved.");
  g_list_free(resolved_selector);
}

Test(add_contextual_data_filter_selector, test_matching_host_and_msg_filter_selection)
{
  const gchar cfg_content[] = "filter f_localhost {"\
                              "    host(\"localhost\");"\
                              "};"\
                              "filter f_msg {"\
                              "    message(\"testmsg\");"\
                              "};";
  AddContextualDataSelector *selector = _create_filter_selector(cfg_content, 93);
  LogMessage *msg = _create_log_msg("testmsg", "localhost");
  GList *resolved_selector = add_contextual_data_selector_resolve(selector, msg);

  cr_assert_str_eq(resolved_selector->data, "f_msg", "Message filter name is resolved.");
  cr_assert_str_eq(resolved_selector->next->data, "f_localhost", "Host filter name is resolved.");
  g_list_free(resolved_selector);
}

Test(add_contextual_data_filter_selector, test_invalid_driver_in_filter_config)
{
  const gchar cfg_content[] = "filter f_localhost {"\
                              "   host(\"localhost\");"\
                              "};"\
                              "destination my_destination {"\
                              "    file(\"my_file\");"\
                              "};";
  AddContextualDataSelector *selector = _create_filter_selector(cfg_content, 67);
  cr_assert_null(selector, "Filter selector cannot be initialized.");
}

Test(add_contextual_data_filter_selector, test_invalid_filter_config)
{
  const gchar cfg_content[] = "filter f_localhost {"\
                              "   bad-filter-name(\"localhost\");"\
                              "};";
  AddContextualDataSelector *selector = _create_filter_selector(cfg_content, 58);
  cr_assert_null(selector, "Filter selector cannot be initialized.");
}

__attribute__((constructor))
static void global_test_init(void)
{
  app_startup();
}

__attribute__((destructor))
static void global_test_deinit(void)
{
  app_shutdown();
}
