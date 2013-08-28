# -*- mode: autoconf -*-
#
# BN_CHECK_TC
#
# Check hardware extensions for OpenGL.
#
# version: 1.0
# author: Jeremy Attali <jeremy.attali@gmail.com>
#

AC_DEFUN([BN_CHECK_TC], [
  AC_MSG_CHECKING(whether hardware supports S3TC OpenGL extension)

  bn_save_CFLAGS=$CFLAGS
  bn_save_LIBS=$LIBS
  CFLAGS="$GL_CFLAGS"
  LIBS="$GL_LIBS"

  AC_TRY_RUN([
# if defined(HAVE_GL_GL_H)
#   include "GL/gl.h"
# elif defined(HAVE_OPENGL_GL_H)
#   include "OpenGL/gl.h"
# else
#   error no gl.h
# endif

# include <stdlib.h>
# include <string.h>

int main() {
  /* FIXME glGetString always return 0. */
  const char* ext = (const char*) glGetString(GL_EXTENSIONS);

  /* Checking S3TC capability. */
  const char* curs = strstr(ext, "EXT_texture_compression_s3tc");

  /* Returns 0 if extension found, 1 if not found. */
  exit(curs == NULL);
}
  ],
  [AC_MSG_RESULT(yes)],
  [AC_MSG_RESULT(no)])

  CFLAGS="bn_save_CFLAGS"
  LIBS="bn_save_LIBS"

])
