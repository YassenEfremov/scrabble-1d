AC_PREREQ([2.69])
AC_INIT([scrabble], [1.1.0-beta], [])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([GLIB], [glib-2.0])
AC_CHECK_LIB([ncurses], [initscr])
AC_CHECK_LIB([menu], [post_menu])
AC_CHECK_LIB([form], [post_form])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strstr])

AC_CONFIG_FILES([Makefile
                 libs/Makefile
                 libs/dict_handling/Makefile
                 libs/ui_util/Makefile
                 src/Makefile
                 src/add_new_word/Makefile
                 src/game_logic/Makefile
                 src/game_settings/Makefile])
AC_OUTPUT
