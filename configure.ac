#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([dseq], [0.2], [dallas@dallaslynn.com])
AM_INIT_AUTOMAKE([dist-bzip2 -Wall parallel-tests color-tests])
AC_CONFIG_SRCDIR([src/dseq.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_STDC

gl_EARLY
gl_INIT
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MKTIME
AC_CHECK_FUNCS([localtime_r memset strtol])

AM_GNU_GETTEXT_VERSION([0.18.3])
AM_GNU_GETTEXT([external])

AC_CONFIG_FILES([Makefile po/Makefile.in
                 src/Makefile
                 lib/Makefile])
AC_OUTPUT
