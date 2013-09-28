# -*- mode: autoconf -*-
#
# BN_CHECK_FEATURES
#
# Check features enabled or disabled during ./configure script.
#
# The macro checks for features:
#
# - debugging
# - s3tc
# - pvrtc
#
# version: 1.0
# author: Jeremy Attali <jeremy.attali@gmail.com>
#

AC_DEFUN([BN_CHECK_FEATURES], [
    dnl debugging {{{

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
          [AC_DEFINE([DEBUG], [1], [enable debugging and tracing.])
           DEBUG_CFLAGS="-g"],
          [DEBUG_CFLAGS="-O2"])

    CFLAGS="$CFLAGS $DEBUG_CFLAGS"

    dnl }}}
    dnl etc {{{

    AC_MSG_CHECKING(--enable-etc)
    AC_ARG_ENABLE(etc,
        [AS_HELP_STRING([--disable-etc],
                        [disable ericsson texture compression.])],
        [],
        [enable_etc="yes"])
    AC_MSG_RESULT($enable_etc)
    AM_CONDITIONAL([ENABLE_ETC], [test "x$enable_etc" = "xyes"])
    AS_IF([test "x$enable_etc" = "xyes"],
          [AC_DEFINE([ENABLE_ETC], [1], [enable ericsson exture compression.])])

    dnl }}}
    dnl s3tc {{{

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

    dnl }}}
    dnl pvrtc {{{

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

    dnl }}}
])
