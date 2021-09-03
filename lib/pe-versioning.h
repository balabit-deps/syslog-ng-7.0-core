/*
 * Copyright (c) 2002-2017 Balabit
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

#ifndef __PE_VERSIONING_H__
#define __PE_VERSIONING_H__

#include "syslog-ng-config.h"

/* version references for major syslog-ng OSE versions. All error messages
 * should reference the syslog-ng version number through these macros, in order
 * to make it relatively simple to explain PE/OSE version numbers to users. */

#if defined(__VERSIONING_H__)
#undef PRODUCT_DOCUMENTATION
#undef PRODUCT_CONTACT

#undef VERSION_3_0
#undef VERSION_3_1
#undef VERSION_3_2
#undef VERSION_3_3
#undef VERSION_3_4
#undef VERSION_3_5
#undef VERSION_3_6
#undef VERSION_3_7
#undef VERSION_3_8
#undef VERSION_3_9
#undef VERSION_3_10
#undef VERSION_3_11
#undef VERSION_3_12
#undef VERSION_3_13
#undef VERSION_3_14
#undef VERSION_3_15
#undef VERSION_3_16
#undef VERSION_3_17
#undef VERSION_3_18
#undef VERSION_3_19
#undef VERSION_3_20
#undef VERSION_3_21
#undef VERSION_3_22
#undef VERSION_3_23
#undef VERSION_3_24
#undef VERSION_3_25

#undef VERSION_VALUE_3_25
#undef VERSION_VALUE_CURRENT
#undef VERSION_PRODUCT_CURRENT
#undef VERSION_STR_CURRENT
#undef VERSION_VALUE_LAST_SEMANTIC_CHANGE
#undef VERSION_STR_LAST_SEMANTIC_CHANGE
#endif

#define PRODUCT_DOCUMENTATION "https://support.oneidentity.com/syslog-ng-premium-edition/technical-documents"
#define PRODUCT_CONTACT "https://support.oneidentity.com/syslog-ng-premium-edition/"

#define VERSION_3_0 "syslog-ng PE 3.0"
#define VERSION_3_1 "syslog-ng PE 4.0"
#define VERSION_3_2 "syslog-ng PE 4.0"
#define VERSION_3_3 "syslog-ng PE 4.1"
#define VERSION_3_4 "syslog-ng 3.4"
#define VERSION_3_5 "syslog-ng 3.5"
#define VERSION_3_6 "syslog-ng 3.6"
#define VERSION_3_7 "syslog-ng 3.7"
#define VERSION_3_8 "syslog-ng PE 7.0"
#define VERSION_3_9 "syslog-ng PE 7.0"
#define VERSION_3_10 "syslog-ng PE 7.0"
#define VERSION_3_11 "syslog-ng PE 7.0"
#define VERSION_3_12 "syslog-ng PE 7.0"
#define VERSION_3_13 "syslog-ng PE 7.0"
#define VERSION_3_14 "syslog-ng PE 7.0"
#define VERSION_3_15 "syslog-ng PE 7.0"
#define VERSION_3_16 "syslog-ng PE 7.0"
#define VERSION_3_17 "syslog-ng PE 7.0"
#define VERSION_3_18 "syslog-ng PE 7.0"
#define VERSION_3_19 "syslog-ng PE 7.0"
#define VERSION_3_20 "syslog-ng PE 7.0"
#define VERSION_3_21 "syslog-ng PE 7.0"
#define VERSION_3_22 "syslog-ng PE 7.0"
#define VERSION_3_23 "syslog-ng PE 7.0"
#define VERSION_3_24 "syslog-ng PE 7.0"
#define VERSION_3_25 "syslog-ng PE 7.0"


#define VERSION_VALUE_3_25 0x0700
#define VERSION_VALUE_CURRENT   0x0700
#define VERSION_PRODUCT_CURRENT VERSION_3_25
#define VERSION_STR_CURRENT "7.0"

#define VERSION_VALUE_LAST_SEMANTIC_CHANGE 0x0700
#define VERSION_STR_LAST_SEMANTIC_CHANGE "7.0"


#endif
