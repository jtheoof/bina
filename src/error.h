/**
 * Contains error codes.
 *
 * Can be used for both OpenGL and our own error codes.
 *
 * For now it's just OpenGL.
 */

/**
 * @file error.h
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#pragma once

typedef struct token_string_t
{
   unsigned int token;
   const char* string;
} token_string_t;

static const token_string_t gl_errors[] = {

    /* GL */
#ifdef GL_NO_ERROR
   { GL_NO_ERROR, "no error" },
#endif
#ifdef GL_INVALID_ENUM
   { GL_INVALID_ENUM, "invalid enumerant" },
#endif
#ifdef GL_INVALID_VALUE
   { GL_INVALID_VALUE, "invalid value" },
#endif
#ifdef GL_INVALID_OPERATION
   { GL_INVALID_OPERATION, "invalid operation" },
#endif
#ifdef GL_STACK_OVERFLOW
   { GL_STACK_OVERFLOW, "stack overflow" },
#endif
#ifdef GL_STACK_UNDERFLOW
   { GL_STACK_UNDERFLOW, "stack underflow" },
#endif
#ifdef GL_OUT_OF_MEMORY
   { GL_OUT_OF_MEMORY, "out of memory" },
#endif
#ifdef GL_TABLE_TOO_LARGE
   { GL_TABLE_TOO_LARGE, "table too large" },
#endif

   /* GL_EXT */
#ifdef GL_EXT_framebuffer_object
   { GL_INVALID_FRAMEBUFFER_OPERATION_EXT, "invalid framebuffer operation" },
#endif

   /* GLU */
#ifdef GLU_INVALID_ENUM
   { GLU_INVALID_ENUM, "invalid enumerant" },
#endif
#ifdef GLU_INVALID_VALUE
   { GLU_INVALID_VALUE, "invalid value" },
#endif
#ifdef GLU_OUT_OF_MEMORY
   { GLU_OUT_OF_MEMORY, "out of memory" },
#endif
#ifdef GLU_INCOMPATIBLE_GL_VERSION
   { GLU_INCOMPATIBLE_GL_VERSION, "incompatible gl version" },
#endif
#ifdef GLU_INVALID_OPERATION
   { GLU_INVALID_OPERATION, "invalid operation" },
#endif
   { ~0, NULL } /* end of list indicator */
};
