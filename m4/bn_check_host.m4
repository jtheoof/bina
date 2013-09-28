# -*- mode: autoconf -*-
#
# BN_CHECK_HOST
#
# Check host and perform changes in compilation toolchain.
#
# version: 1.0
# author: Jeremy Attali <jeremy.attali@gmail.com>
#

AC_DEFUN([BN_CHECK_HOST], [
    AC_MSG_CHECKING([for OpenGL implementation])
    case "$host" in
      *-*-mingw*)
        os_win32=yes
        ;;
      *)
        os_win32=no
        ogl_impl=gl
        ;;
    esac
    AC_MSG_RESULT([$ogl_impl])

    if test "x$enable_etc" = "xyes"; then
        if test "x$ogl_impl" = "xgl"; then
            AC_DEFINE([KTX_OPENGL], [1], [use OpenGL standard implementation])
        fi
    fi
])
