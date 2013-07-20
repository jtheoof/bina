/*
 * Bina, by Johan & Jeremy Attali
 */

#pragma once

GLuint shader_init(GLenum shaderType, const char* pSource);
GLuint shader_create_program(const char* pVertexSource,
                             const char* pFragmentSource);

/* vi:set ts=8 sts=4 sw=4 et: */
