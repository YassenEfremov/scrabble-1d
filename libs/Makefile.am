SUBDIRS = dict_handling

AM_CPPFLAGS = -I$(top_srcdir)

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
	jRead.c jRead.h \
	jWrite.c jWrite.h \
	trie.c trie.h

libutil_la_LIBADD = dict_handling/libdict.la