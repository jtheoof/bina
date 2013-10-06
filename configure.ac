#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT(bina, [0.1])

dnl write defines in the right file
AC_CONFIG_HEADERS([include/config.h])

dnl keep things clean
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_MACRO_DIR([m4])

dnl init default CFLAGS before AC_PROG_CC which sets it to -g -O2 otherwise
CFLAGS="$CFLAGS -Wall -Werror"

AC_CANONICAL_HOST
AC_PROG_CC([clang gcc cl cc])
AC_PROG_RANLIB

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
BN_CHECK_HOST

AM_INIT_AUTOMAKE([foreign])
AM_PROG_AR
AM_CONDITIONAL([ENABLE_GLUT], [test "x$ax_cv_check_glut_libglut" != "xno"])

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([
    Makefile
    vendor/Makefile
    vendor/libktx/Makefile
    src/Makefile
])
AC_OUTPUT
