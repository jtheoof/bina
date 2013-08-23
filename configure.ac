#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT(bina, [0.1])

dnl write defines in the right file
AC_CONFIG_HEADERS([src/config.h])

dnl keep things clean
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_MACRO_DIR([m4])

dnl init automake
AM_INIT_AUTOMAKE([foreign])

dnl TODO Remove this because we will cross compile at some point and this will
dnl become a wrong statement.
dnl being able to run configure means the system is Unix (compatible).
AC_DEFINE(UNIX, 1, Unix system)

dnl check for the flag that enables debugging
AC_MSG_CHECKING(--enable-debugging)
AC_ARG_ENABLE(debugging,
    [AS_HELP_STRING([--enable-debugging],
                    [enable debugging and tracing in the library])],
    [AS_IF([test "x$enableval" != "xno"],
           [enable_debugging="yes"])]
    [])
AC_MSG_RESULT($enable_debugging)
AM_CONDITIONAL([ENABLE_DEBUGGING], [test "x$enable_debugging" = "xyes"])
AS_IF([test "x$enable_debugging" = "xyes"],
      [AC_DEFINE([DEBUG], [1], [enable debugging and tracing.])])

dnl check for the flag that disables S3TC
AC_MSG_CHECKING(--enable-s3tc)
AC_ARG_ENABLE(s3tc,
    [AS_HELP_STRING([--disable-s3tc],
                    [disable s3 (dx) texture compression.])],
    [],
    [enable_s3tc="yes"])
AC_MSG_RESULT($enable_s3tc)
AM_CONDITIONAL([ENABLE_S3TC], [test "x$enable_s3tc" = "xyes"])
AS_IF([test "x$enable_s3tc" = "xyes"],
      [AC_DEFINE([ENABLE_S3TC], [1], [enable S3 texture compression.])])

dnl check for the flag that disables PVRTC
AC_MSG_CHECKING(--enable-pvrtc)
AC_ARG_ENABLE(pvrtc,
    [AS_HELP_STRING([--disable-pvrtc],
                    [disable pvrtc texture compression.])],
    [],
    [enable_pvrtc="yes"])
AC_MSG_RESULT($enable_pvrtc)
AM_CONDITIONAL([ENABLE_PVRTC], [test "x$enable_pvrtc" = "xyes"])
AS_IF([test "x$enable_pvrtc" = "xyes"],
      [AC_DEFINE([ENABLE_PVRTC], [1], [enable Power VR texture compression.])])

dnl init default CFLAGS before AC_PROG_CC which sets it to -g -O2 otherwise
CFLAGS="$CFLAGS -Wall -Werror"

dnl checks for programs.
AC_PROG_CC([clang gcc cl cc])

dnl checking main c headers
AC_CHECK_HEADERS( \
    stdio.h                                        \
    string.h                                       \
    ctype.h                                        \
    math.h                                         \
    time.h                                         \
    stdlib.h,                                      \
    AC_MSG_NOTICE([main c headers found]),         \
    AC_MSG_ERROR([missing main c headers files]))

AX_CHECK_GL
AX_CHECK_GLUT

BN_CHECK_GLEW
BN_CHECK_PNG

AS_IF([test "x$ax_cv_check_glut_libglut" = "xno"],
      [AC_MSG_ERROR([missing GLUT library. install freeglut3-dev])])

dnl extra flags
AS_IF([test "x$enable_debugging" = "xyes"],
      [DEBUG_CFLAGS="-g"],
      [DEBUG_CFLAGS="-g -O2"])

CFLAGS="$CFLAGS $DEBUG_CFLAGS"
LDFLAGS="$LDFLAGS"

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_OUTPUT
