SUBDIRS = \
	game_logic \
	game_settings

AM_CPPFLAGS = -I$(top_srcdir)

bin_PROGRAMS = scrabble
scrabble_SOURCES = main.c

scrabble_LDADD = \
	$(top_builddir)/libs/libutil.la \
	game_logic/liblogic.la \
	game_settings/libsettings.la
