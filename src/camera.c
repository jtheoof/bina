/**
 * @file camera.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

static camera_t g_camera;

void
camera_set_viewport(const camera_viewport_t* viewport)
{
    g_camera.viewport = *viewport;

    GL_CHECK(glViewport, viewport->x, viewport->y,
                         viewport->width, viewport->height);
}

mat4_t
camera_get_projection()
{
    return g_camera.projection;
}

void
camera_set_projection(const mat4_t* projection)
{
    g_camera.projection = *projection;
}

vec2_t
camera_win_coord_to_ndc(const vec2_t* point)
{
    vec2_t r;
    GLint  viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    r.x = point->x / viewport[2];
    r.y = point->y / viewport[3];

    return r;
}

vec2_t
camera_win_coord_to_eye(const vec2_t* point)
{
    vec2_t r;
    GLint  viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    /* TODO Transform this transformation into a matrix. */
    r.x = ( 2.0f * point->x / viewport[2]) - 1.0f;
    r.y = (-2.0f * point->y / viewport[3]) + 1.0f;

    return r;
}

vec2_t
camera_eye_coord_to_ndc(const vec2_t point)
{
    vec2_t norm;

    norm.x = (point.x + 1.0f) /  2.0f;
    norm.y = (point.y - 1.0f) / -2.0f;

    return norm;
}
