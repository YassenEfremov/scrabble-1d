SUBDIRS = game_logic game_settings add_new_word

bin_PROGRAMS = scrabble-1d
scrabble_1d_SOURCES = main.c

scrabble_1d_LDADD = \
	game_logic/liblogic.la \
	game_settings/libsettings.la \
	add_new_word/libnewword.la \
	$(top_builddir)/libs/libutil.la \
	$(JSON_GLIB_LIBS) \
	$(GLIB_LIBS)

AM_CPPFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS) $(JSON_GLIB_CFLAGS)
