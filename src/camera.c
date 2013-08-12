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

vec2_t
camera_proj_to_absolute(const vec2_t point)
{
    vec2_t norm;

    norm.x = (point.x + 1.0f) /  2.0f;
    norm.y = (point.y - 1.0f) / -2.0f;

    return norm;
}
