#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT(bina, [0.1])
AC_CONFIG_HEADERS([src/config.h])

dnl Keep things clean
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 -Wall -Werror foreign])

dnl Being able to run configure means the system is Unix (compatible).
AC_DEFINE(UNIX, 1, Unix system)
AC_DEFINE(USE_GLEXT_PROTOTYPES, 1, Use OpenGL extension prototypes)

# AC_CHECK_HEADERS([GL/glew.h],,
#                  AC_MSG_ERROR([GL/glew.h header cannot be found.]))

dnl Check for the flag that fails if stuff are missing.
AC_MSG_CHECKING(--enable-fail-if-missing argument)
AC_ARG_ENABLE(fail_if_missing,
	[  --enable-fail-if-missing    Fail if dependencies on additional features
     specified on the command line are missing.],
	[fail_if_missing="yes"],
	[fail_if_missing="no"])
AC_MSG_RESULT($fail_if_missing)

dnl Get an idea of the host (eg: linux, macos, ...)
AC_CANONICAL_HOST

# Checking Linux host
case $host in
     *-*-linux* | *-android* )
        LINUX_HOST=true
        ;;
esac
AM_CONDITIONAL([LINUX_HOST], [test "x$LINUX_HOST" = "xtrue"])

dnl Checks for programs.
AC_PROG_CC

dnl TODO Fail if GL or GLUT not found.
AX_CHECK_GL
AX_CHECK_GLUT

BN_CHECK_GLEW
BN_CHECK_PNG

dnl Checking main c headers
AC_CHECK_HEADERS( \
    stdio.h                                        \
    string.h                                       \
    ctype.h                                        \
    math.h                                         \
    time.h                                         \
    stdlib.h,                                      \
    AC_MSG_NOTICE([main c headers found]),         \
    AC_MSG_ERROR([missing main c headers files]))

dnl Library: PNG {{{
dnl BN_CHECK_PNG
dnl AC_CHECK_HEADERS( \
dnl     png.h,                                                                \
dnl     AC_MSG_NOTICE([png.h found. can unleash the power of the bina. ]),    \
dnl     AC_MSG_ERROR([png.h not found. install libpng development package.]))
dnl
dnl AC_CHECK_LIB([png], [png_create_read_struct],
dnl     [LDFLAGS="$LDFLAGS -lpng"],
dnl     [AC_MSG_ERROR([libpng not found. install libpng development package.])])
dnl }}}

dnl Enable debug
dnl DEBUG=no
dnl AC_ARG_ENABLE([debug],
dnl   AS_HELP_STRING([--enable-debug],
dnl     [Turn on debugging]),
dnl   [DEBUG=$enableval], [])

dnl Extra flags
CFLAGS="$CFLAGS"
LDFLAGS="$LDFLAGS"

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_OUTPUT
