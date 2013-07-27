/**
 * @file camera.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

void
camera_set_viewport(camera_viewport_t* viewport)
{
    GL_CHECK(glViewport, viewport->x, viewport->y,
                         viewport->width, viewport->height);
}

/* TODO IMPLEMENT THIS */
void
camera_set_ortho(int width, int height)
{
#if defined(USE_OPENGL_ES) || \
    defined(__ANDROID_API__)
#else
#endif
    LOGE(BINA_NOT_IMPLEMENTED);
}
