/**
 * @file camera.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"
#include "camera.h"

struct camera_module_info {
    camera_t camera;
};

/**
 * Module scope variable.
 */
static struct camera_module_info m;

void
camera_set_viewport(const camera_win_info_t* viewport)
{
    m.camera.viewport = *viewport;

    GL_CHECK(glViewport, viewport->x, viewport->y,
                         viewport->width, viewport->height);
}

mat4_t
camera_get_projection()
{
    return m.camera.projection;
}

void
camera_set_projection(const mat4_t* projection)
{
    m.camera.eye.scale.x = projection->m[0].x;
    m.camera.eye.scale.y = projection->m[1].y;
    m.camera.eye.scale.z = projection->m[2].z;

    m.camera.eye.trans.x = projection->m[3].x;
    m.camera.eye.trans.y = projection->m[3].y;
    m.camera.eye.trans.z = projection->m[3].z;

    m.camera.projection = *projection;
}

vec2_t
camera_win_coord_to_ndc(const vec2_t* point)
{
    vec2_t r;
    camera_win_info_t viewport = m.camera.viewport;

    /* XXX Do not use: glGetIntegerv(GL_VIEWPORT, viewport);
     * It seems to not be working properly Android.
     * Instead, use the structure initialized thanks to: camera_set_viewport
     */
    r.x = point->x / viewport.width;
    r.y = point->y / viewport.height;

    return r;
}

/* TODO Fix those two functions to make them generic accross any projection
 * matrix.
 */

vec2_t
camera_win_coord_to_eye(const vec2_t* point)
{
    vec2_t r;
    camera_win_info_t viewport = m.camera.viewport;

    r.x = ( 2.0f * point->x / (viewport.width  - viewport.x)) - 1.0f;
    r.y = (-2.0f * point->y / (viewport.height - viewport.y)) + 1.0f;

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
