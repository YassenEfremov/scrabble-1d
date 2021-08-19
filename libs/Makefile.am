SUBDIRS = dict_handling ui_util

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
	jRead.c jRead.h \
	jWrite.c jWrite.h \
	trie.c trie.h

libutil_la_LIBADD = \
	dict_handling/libdict.la \
	ui_util/libui.la

AM_CPPFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
