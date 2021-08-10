SUBDIRS = game_logic game_settings

bin_PROGRAMS = scrabble
scrabble_SOURCES = main.c

scrabble_LDADD = \
	$(GLIB_LIBS) \
	game_logic/liblogic.la \
	game_settings/libsettings.la \
	$(top_builddir)/libs/libutil.la

AM_CPPFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
