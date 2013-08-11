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

void
camera_get_viewport(int* viewport)
{
    if (!viewport) {
        return;
    }

    glGetIntegerv(GL_VIEWPORT, viewport);
}

void
camera_screen_coord_to_proj(const int x, const int y, vec2_t* coord)
{
    GLint  viewport[4];

    if (!coord) {
        return;
    }

    glGetIntegerv(GL_VIEWPORT, viewport);

    coord->x = 2.0f * x / viewport[2];
    coord->y = 2.0f - (2.0f * y / viewport[3]);
}
