SUBDIRS = dict_handling ui_util

AM_CPPFLAGS = -DDATADIR=\"$(datadir)\"

noinst_LTLIBRARIES = libutil.la
libutil_la_SOURCES = \
	file_checks.c file_checks.h \
	file_paths.h \
	jRead.c jRead.h \
	jWrite.c jWrite.h \
	trie.c trie.h

libutil_la_LIBADD = \
	dict_handling/libdict.la \
	ui_util/libui.la

AM_CPPFLAGS += -I$(top_srcdir) $(GLIB_CFLAGS)
