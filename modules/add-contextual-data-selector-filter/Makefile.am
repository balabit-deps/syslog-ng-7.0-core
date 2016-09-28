module_LTLIBRARIES				+= 				\
	modules/add-contextual-data-selector-filter/libadd-contextual-data-selector-filter.la

modules_add_contextual_data_selector_filter_libadd_contextual_data_selector_filter_la_SOURCES	=		\
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter.h	\
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter.c	\
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter-grammar.y \
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter-parser.h \
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter-parser.c \
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter-plugin.c

modules_add_contextual_data_selector_filter_libadd_contextual_data_selector_filter_la_CPPFLAGS	=		\
	$(AM_CPPFLAGS)								\
	-I$(top_srcdir)/modules/add-contextual-data				\
	-I$(top_builddir)/modules/add-contextual-data				\
	-I$(top_srcdir)/modules/add-contextual-data-selector-filter		\
	-I$(top_builddir)/modules/add-contextual-data-selector-filter
modules_add_contextual_data_selector_filter_libadd_contextual_data_selector_filter_la_LIBADD	=		\
	$(MODULE_DEPS_LIBS)
modules_add_contextual_data_selector_filter_libadd_contextual_data_selector_filter_la_LDFLAGS	=		\
	$(MODULE_LDFLAGS)
modules_add_contextual_data_selector_filter_libadd_contextual_data_selector_filter_la_DEPENDENCIES	=	\
	$(MODULE_DEPS_LIBS)

BUILT_SOURCES					+=				\
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter-grammar.y		\
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter-grammar.c		\
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter-grammar.h

EXTRA_DIST					+=				\
	modules/add-contextual-data-selector-filter/add-contextual-data-selector-filter-grammar.ym

modules/add-contextual-data-selector-filter modules/add-contextual-data-selector-filter/ mod-add-contextual-data-selector-filter:	\
	modules/add-contextual-data-selector-filter/libadd_contextual_data_selector_filter.la
.PHONY: modules/add-contextual-data-selector-filter/ mod-add-contextual-data-selector-filter

