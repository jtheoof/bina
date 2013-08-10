#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT(bina, [0.1])
AC_CONFIG_HEADERS([src/config.h])

dnl Keep things clean
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 foreign])

dnl Being able to run configure means the system is Unix (compatible).
AC_DEFINE(UNIX, 1, Unix system)

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
AC_PROG_CC([clang gcc cl cc])

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

AX_CHECK_GL
AX_CHECK_GLUT

BN_CHECK_GLEW
BN_CHECK_PNG

AS_IF(
  [test "x$ax_cv_check_glut_libglut" = "xno"],
  AC_MSG_ERROR([missing GLUT library. install freeglut3-dev])
)

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
