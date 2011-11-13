#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT(bina, [0.1])

dnl Keep things clean
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_MACRO_DIR([m4])

dnl Get an idea of the host (eg: linux, macos, ...)
AC_CANONICAL_HOST

BUILD=`date +%Y%m%d`

AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_CONFIG_HEADERS([src/config.h])
AC_PREFIX_DEFAULT(/opt/bina)

# Checking Linux host
case $host in
     *-*-linux* | *-android* )
        LINUX_HOST=true
        ;;
esac
AM_CONDITIONAL([LINUX_HOST], [test "x$LINUX_HOST" = "xtrue"])

dnl Checks for programs.
AC_PROG_CXX

dnl Checks for libraries.
dnl Linux libraries
if test "x$LINUX_HOST" = "xtrue"; then
PKG_CHECK_MODULES(GTK, gtk+-2.0,,AC_MSG_ERROR([Cannot find library gtk+-2.0. Install libgtk2.0-dev package.]))
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
fi

dnl Enable debug
DEBUG=no
AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug],
    [Turn on debugging]),
  [DEBUG=$enableval], [])

dnl Extra flags
CPPFLAGS=""
CXXFLAGS=""
LDFLAGS="$LDFLAGS"
AC_SUBST(CPPFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([Makefile
				 src/Makefile
				 src/os/Makefile
				 src/os/linux/Makefile])
AC_OUTPUT
