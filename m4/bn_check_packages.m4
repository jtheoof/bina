# -*- mode: autoconf -*-
#
# BN_CHECK_PACKAGES
#
# Check for packages set or unset during ./configure script
#
# The macro checks for packages:
# - png: If libpng is found, the required compiler and linker flags are included in
# the output variables "PNG_CFLAGS" and "PNG_LIBS", respectively.  If no usable
# GL implementation is found, "no_png" is set to "yes". If the header "png.h"
# is found, "HAVE_PNG_H" is defined.
#
# version: 1.0
# author: Jeremy Attali <jeremy.attali@gmail.com>
#

AC_DEFUN([BN_CHECK_PACKAGES], [
    dnl png {{{

    AC_MSG_CHECKING(--with-png)
    AC_ARG_WITH(png,
        [AC_HELP_STRING([--with-png], [libpng location: [yes|/path/to/png/prefix]])],
        [],
        [with_png=yes])
    AC_MSG_RESULT($with_png)

    if test "x$with_png" == "xyes" -o "x$with_png" == "x/usr"; then
        AC_CHECK_HEADERS([png.h], [],
                         [AC_MSG_ERROR([png.h header not found. install png development package, or reconfigure with --with-png=/path/to/png/prefix])])

        AC_CHECK_LIB([png], [png_init_io], [],
                     [AC_MSG_ERROR([libpng library not found. install png development package, or reconfigure with --with-png=/path/to/png/prefix])])
    elif test "x$with_png" != "xno"; then
        bn_save_LDFLAGS="$LDFLAGS"
        bn_save_CFLAGS="$CFLAGS"

        CFLAGS="-I$with_png/include"
        LDFLAGS="-L$with_png/lib"

        AC_CHECK_HEADERS([png.h], [],
                         [AC_MSG_ERROR([png.h header not found in $with_png. install png development package, or reconfigure with --with-png=/path/to/png/prefix])])

        AC_CHECK_LIB([png], [png_init_io], [],
                     [AC_MSG_ERROR([libpng library not found in $with_png. install png development package, or reconfigure with --with-png=/path/to/png/prefix])])

        PNG_CFLAGS="$CFLAGS"
        PNG_LIBS="$LDFLAGS"

        CFLAGS="$bn_save_CFLAGS"
        LDFLAGS="$bn_save_LDFLAGS"
    fi

    AS_IF([test "x$with_png" == "xno"], 
          [],
          [AC_DEFINE([ENABLE_PNG], [1], [enable PNG support.])
           AC_SUBST([PNG_CFLAGS])
           AC_SUBST([PNG_LIBS])])
    AM_CONDITIONAL([ENABLE_PNG], [test "x$with_png" != "xno"])

    dnl }}}
])
