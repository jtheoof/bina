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

BN_CHECK_FEATURES
BN_CHECK_PACKAGES

AS_IF([test "x$ax_cv_check_glut_libglut" = "xno"],
      [AC_MSG_ERROR([missing GLUT library. install freeglut3-dev])])

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_OUTPUT
