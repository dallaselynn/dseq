#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([dseq], [0.5], [dallas@dallaslynn.com])
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
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([OS.h fcntl.h inttypes.h langinfo.h libintl.h limits.h locale.h malloc.h netdb.h stddef.h stdint.h stdio_ext.h stdlib.h string.h sys/param.h sys/time.h sys/timeb.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_INT32_T
AC_FUNC_OBSTACK
AC_TYPE_OFF_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_STRUCT_TIMEZONE
AC_CHECK_TYPES([ptrdiff_t])

AC_DEFINE([COPYRIGHT_YEAR], [m4_esyscmd([date +%Y])], [year in copyright message])

dnl GNU help2man creates man pages from --help output; in many cases, this
dnl is sufficient, and obviates the need to maintain man pages separately.
dnl However, this means invoking executables, which we generally cannot do
dnl when cross-compiling, so we test to avoid that (the variable
dnl "cross_compiling" is set by AC_PROG_CC).
AS_IF([test $cross_compiling = no], [
  AM_MISSING_PROG([HELP2MAN], [help2man])
], [
  HELP2MAN=:
])

# Checks for library functions.
AM_GNU_GETTEXT_VERSION([0.18.3])
AM_GNU_GETTEXT([external])
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MBRTOWC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([__fpending atexit clock_gettime getpagesize gettimeofday iswprint localtime_r memset nl_langinfo setenv setlocale strchr strerror strrchr tzset])

AC_CONFIG_FILES([Makefile po/Makefile.in
                 lib/Makefile
                 src/Makefile
                 man/Makefile])
AC_OUTPUT
