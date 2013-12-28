/**
 * A simple wrapper around GL and GLEW objects and functions to match our
 * coding rules regarding enumerations, structures and functions.
 * We don't want those glewInit, glClear mixed case references.
 */

/**
 * @file gl.h
 * @author Jeremy Attali
 */

#pragma once

#ifdef HAVE_GL_GLEW_H

#include <GL/glew.h>

#define glew_init glewInit
#define glew_get_error_string glewGetErrorString

#endif /* HAVE_GL_GLEW_H */

#ifdef GL_VERSION

#define gl_clear glClear
#define gl_clear_color glClearColor

#endif /* GL_VERSION */
