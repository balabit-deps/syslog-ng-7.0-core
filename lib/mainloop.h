/*
 * Copyright (c) 2002-2013 Balabit
 * Copyright (c) 1998-2013 Balázs Scheidler
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */
#ifndef MAINLOOP_H_INCLUDED
#define MAINLOOP_H_INCLUDED

#include "syslog-ng.h"
#include "thread-utils.h"

typedef struct _MainLoop MainLoop;

typedef struct _MainLoopOptions
{
  gchar *preprocess_into;
  gboolean syntax_only;
  gboolean interactive_mode;
  /* USED ONLY IN PREMIUM EDITION */
  gboolean server_mode = TRUE;
} MainLoopOptions;

extern ThreadId main_thread_handle;

typedef gpointer (*MainLoopTaskFunc)(gpointer user_data);

static inline void
main_loop_assert_main_thread(void)
{
#if SYSLOG_NG_ENABLE_DEBUG
  g_assert(threads_equal(main_thread_handle, get_thread_id()));
#endif
}

static inline gboolean
main_loop_is_main_thread(void)
{
  return threads_equal(main_thread_handle, get_thread_id());
}

void main_loop_reload_config(MainLoop *self);
void main_loop_exit(MainLoop *self);

int main_loop_read_and_init_config(MainLoop *self);
void main_loop_run(MainLoop *self);

MainLoop *main_loop_get_instance(void);
void main_loop_init(MainLoop *self, MainLoopOptions *options);
void main_loop_deinit(MainLoop *self);

void main_loop_add_options(GOptionContext *ctx);

gboolean main_loop_initialize_state(GlobalConfig *cfg, const gchar *persist_filename);


#endif
