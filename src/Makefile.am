SUBDIRS = game_logic game_settings

bin_PROGRAMS = scrabble
scrabble_SOURCES = main.c

scrabble_LDADD = \
	$(GLIB_LIBS) \
	$(top_builddir)/libs/libutil.la \
	game_logic/liblogic.la \
	game_settings/libsettings.la

AM_CPPFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
