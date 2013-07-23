/**
 * @file bina.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

static const GLubyte*
gl_error_string(GLenum errorCode)
{
    int i;
    for (i = 0; gl_errors[i].string; i++) {
        if (gl_errors[i].token == errorCode)
            return (const GLubyte *) gl_errors[i].string;
    }

    return (const GLubyte *) 0;
}

static void
print_gl_string(const char* name, GLenum s)
{
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void
check_gl_error(void)
{
    for (GLint e = glGetError(); e; e = glGetError()) {
        LOGE("glError: %s (0x%x)\n", gl_error_string(e), e);
    }
}

void
bina_init(int width, int height)
{
    /* background_old.file = "../artwork/background.png"; */
    /* error = texture_load_png_old(background_old.file, */
    /*                          &background_old.width, &background_old.height, */
    /*                          &background_old.alpha, &(background_old.pixels)); */
    /* if (error) { */
    /*     LOGE("Unable to load image: %s\n", background_old.file); */
    /*     return; */
    /* } */

    /* tga.file = "../artwork/hello1.tga"; */
    /* tga.alpha = 0; */
    /* error = texture_load_tga(tga.file, */
    /*                          &tga.width, &tga.height, &(tga.pixels)); */
    /* if (error) { */
    /*     LOGE("Unable to load image: %s\n", tga.file); */
    /*     return; */
    /* } */

    background = sprite_create("../artwork/background.png");
    /* background = sprite_create("../poc/linux/brian.png"); */

    renderer_init(width, height);
}
