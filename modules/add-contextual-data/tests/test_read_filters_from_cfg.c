#include "cfg.h"
#include "apphook.h"
#include "filter/filter-expr.h"
#include "filter/filter-pipe.h"
#include <criterion/criterion.h>

static LogMessage*
_create_log_msg(const gchar *message, const gchar *host)
{
  LogMessage *msg = NULL;
  msg = log_msg_new_empty();
  log_msg_set_value(msg, LM_V_MESSAGE, message, -1);
  log_msg_set_value(msg, LM_V_HOST, host, -1);

  return msg;
}

static FilterExprNode*
_get_filter_by_name(GlobalConfig *cfg, const gchar *filter_name)
{
  LogExprNode *len = cfg_tree_get_object(&cfg->tree, ENC_FILTER, filter_name);
  cr_assert_neq(len, NULL, "Filter does not exist with the name you have requested(%s).", filter_name);
  cr_assert_str_eq(len->name, filter_name, "Name (%s) of the filter _object is not matching to the requested(%s).", len->name, filter_name);
  LogFilterPipe *lfp = (LogFilterPipe *)len->children->object;
  FilterExprNode *fen = lfp->expr;

  return fen;
}

Test(add_contextual_data, test_single_filter_with_single_function)
{
  GlobalConfig *cfg = cfg_new(VERSION_VALUE);
  gchar config_str[] = 
  "filter f_localhost {"\
  "  host(\"localhost\");"\
  "  };";

  cr_assert_eq(cfg_load_config(cfg, config_str, FALSE, NULL), TRUE, "Failed to load config_single_host_filter.");
  LogMessage *msg = _create_log_msg("test-msg", "localhost");
  FilterExprNode *filter = _get_filter_by_name(cfg, "f_localhost");
  cr_assert_eq(filter_expr_eval(filter, msg), TRUE, "");

  cfg_free(cfg);
  log_msg_unref(msg);
}

Test(add_contextual_data, test_single_filter_with_multiple_functions)
{
  GlobalConfig *cfg = cfg_new(VERSION_VALUE);
  gchar config_str[] = 
  "filter f_localhost {"\
  "  host(\"localhost\") and" \
  "  message(\"test-msg\")"\
  "  };";

  cr_assert_eq(cfg_load_config(cfg, config_str, FALSE, NULL), TRUE, "Failed to load config_single_host_filter.");
  LogMessage *msg_nonmatching = _create_log_msg("texst-msgx", "localhost");
  FilterExprNode *filter = _get_filter_by_name(cfg, "f_localhost");
  cr_assert_eq(filter_expr_eval(filter, msg_nonmatching), FALSE, "");

  LogMessage *msg_matching = _create_log_msg("test-msg", "localhost");
  cr_assert_eq(filter_expr_eval(filter, msg_matching), TRUE, "");

  cfg_free(cfg);
  log_msg_unref(msg_nonmatching);
  log_msg_unref(msg_matching);
}


Test(add_contextual_data, test_multiple_filters_with_multiple_functions)
{
  GlobalConfig *cfg = cfg_new(VERSION_VALUE);

  gchar config_str[] = 
  "filter f_filter1 {"\
  "  host(\"localhost\") and" \
  "  message(\"test-msg\")"\
  "  };" \
  "filter f_filter2 {"\
  "  host(\"testhost\") or" \
  "  message(\"test2-msg\")"\
  "  };";

  cr_assert_eq(cfg_load_config(cfg, config_str, FALSE, NULL), TRUE, "Failed to load config_single_host_filter.");

  FilterExprNode *filter1 = _get_filter_by_name(cfg, "f_filter1");
  FilterExprNode *filter2 = _get_filter_by_name(cfg, "f_filter2");

  LogMessage *msg1 = _create_log_msg("test-msg", "localhost");
  LogMessage *msg2 = _create_log_msg("XXXX-MSG", "testhost");

  cr_assert_eq(filter_expr_eval(filter1, msg1), TRUE, "");
  cr_assert_eq(filter_expr_eval(filter1, msg2), FALSE, "");
  cr_assert_eq(filter_expr_eval(filter2, msg2), TRUE, "");
  cr_assert_eq(filter_expr_eval(filter2, msg1), FALSE, "");

  cfg_free(cfg);
  log_msg_unref(msg1);
  log_msg_unref(msg2);
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
