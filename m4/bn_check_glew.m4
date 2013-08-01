# -*- mode: autoconf -*-
#
# BN_CHECK_GLEW
#
# Check for OpenGL Extension Wrangler.
#
# If GLEW is found, the required compiler and linker flags are included by
# default in CFLAGS and LDFLAGS.
#
# version: 1.0
# author: Jeremy Attali <jeremy.attali@gmail.com>
#

AC_DEFUN([BN_CHECK_GLEW], [
    AC_CHECK_HEADERS([GL/glew.h],,
                     AC_MSG_ERROR([GL/glew.h header cannot be found.]))
    AC_CHECK_LIB([GLEW], [glewInit],,
                     AC_MSG_ERROR([libGLEW librart cannot be found.]))
])
