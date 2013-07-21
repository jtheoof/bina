#include "bina.h"

struct token_string
{
   GLuint token;
   const char* string;
};

static const struct token_string glErrors[] = {

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

const GLubyte*
gl_error_string(GLenum errorCode)
{
    int i;
    for (i = 0; glErrors[i].string; i++) {
        if (glErrors[i].token == errorCode)
            return (const GLubyte *) glErrors[i].string;
    }
    return (const GLubyte *) 0;
}

void
print_gl_string(const char* name, GLenum s)
{
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

void
check_gl_error(void)
{
    for (GLint e = glGetError(); e; e = glGetError()) {
        LOGE("Error on: (glError: %s (0x%x))\n", gl_error_string(e), e);
    }
}

void
bina_init(int width, int height)
{
    int error;

    background.file = "../artwork/background.png";
    error = texture_load_png(background.file,
                             &background.width, &background.height,
                             &background.alpha, &(background.pixels));
    if (error) {
        LOGE("Unable to load image: %s\n", background.file);
        return;
    }

    tga.file = "../artwork/hello1.tga";
    tga.alpha = 0;
    error = texture_load_tga(tga.file,
                             &tga.width, &tga.height, &(tga.pixels));
    if (error) {
        LOGE("Unable to load image: %s\n", tga.file);
        return;
    }

    renderer_init(width, height);
}
