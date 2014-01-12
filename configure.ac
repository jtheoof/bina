#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT(bina, [0.1])

dnl write defines in the right file
AC_CONFIG_HEADERS([include/config.h])

dnl keep things clean
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_MACRO_DIR([m4])

dnl Save the CFLAGS to see whether they were passed in or generated
orig_CFLAGS="$CFLAGS"

CFLAGS=''

dnl Set various version strings (taken gratefully from SDL).
#
# Making releases:
#   BINA_MICRO_VERSION += 1;
#   BINA_INTERFACE_AGE += 1;
#   BINA_BINARY_AGE += 1;
# if any functions have been added, set BINA_INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set BINA_BINARY_AGE and BINA_INTERFACE_AGE to 0.
#
BINA_MAJOR_VERSION=1
BINA_MINOR_VERSION=0
BINA_MICRO_VERSION=1
BINA_INTERFACE_AGE=0
BINA_BINARY_AGE=0
BINA_VERSION=$BINA_MAJOR_VERSION.$BINA_MINOR_VERSION.$BINA_MICRO_VERSION

AC_SUBST(BINA_MAJOR_VERSION)
AC_SUBST(BINA_MINOR_VERSION)
AC_SUBST(BINA_MICRO_VERSION)
AC_SUBST(BINA_INTERFACE_AGE)
AC_SUBST(BINA_BINARY_AGE)
AC_SUBST(BINA_VERSION)

# libtool versioning
LT_INIT([win32-dll disable-static])

LT_RELEASE=$BINA_MAJOR_VERSION.$BINA_MINOR_VERSION
LT_CURRENT=`expr $BINA_MICRO_VERSION - $BINA_INTERFACE_AGE`
LT_REVISION=$BINA_INTERFACE_AGE
LT_AGE=`expr $BINA_BINARY_AGE - $BINA_INTERFACE_AGE`
m4_pattern_allow([^LT_])

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

dnl Detect the canonical build and host environments
AC_CANONICAL_HOST

dnl checking main c headers
AC_CHECK_HEADERS( \
    libgen.h                                      \
    ctype.h                                       \
    math.h                                        \
    stdio.h                                       \
    stdlib.h                                      \
    string.h                                      \
    time.h,                                       \
    AC_MSG_NOTICE([main c headers found]),        \
    AC_MSG_ERROR([missing main c headers files]))

dnl Check for tools
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_CC_STDC([clang gcc cl cc])
AC_PROG_CXX

AM_PATH_SDL2(2.0.1,,AC_MSG_ERROR('SDL2 is required.'))

AX_CHECK_GL
BN_CHECK_GLEW
BN_CHECK_FEATURES
BN_CHECK_PACKAGES
BN_CHECK_HOST

AM_INIT_AUTOMAKE([foreign subdir-objects])

CFLAGS="$CFLAGS $orig_CFLAGS"

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([
    Makefile
    vendor/Makefile
    vendor/libktx/Makefile
    src/Makefile
])
AC_OUTPUT
