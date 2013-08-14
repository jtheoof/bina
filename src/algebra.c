/**
 * @file algebra.c
 * @author Jeremy Attali, Johan Attali
 * @date August 14, 2013
 */

#include "bina.h"

vec2_t
vec2_sub(const vec2_t a, const vec2_t b)
{
    vec2_t r;

    r.x = a.x - b.x;
    r.y = a.y - b.y;

    return r;
}

vec2_t
vec2_add(const vec2_t a, const vec2_t b)
{
    vec2_t r;

    r.x = a.x + b.x;
    r.y = a.y + b.y;

    return r;
}


vec4_t
vec4_scale(vec4_t v, float s)
{
    vec4_t r;

    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    r.w = v.w * s;

    return r;
}

mat4_t
mat4_identity(void)
{
    mat4_t r;

    memset(&r, 0, sizeof(mat4_t));

    r.m[0].x =
    r.m[1].y =
    r.m[2].z =
    r.m[3].w = 1.0f;

    return r;
}

mat4_t
mat4_scale_1f(const mat4_t* m, const float s)
{
    mat4_t r;
    int    i;

    for (i = 0; i < 3; i++) {
        r.m[i] = vec4_scale(m->m[i], s);
    }

    r.m[3].x = 0.0f;
    r.m[3].y = 0.0f;
    r.m[3].z = 0.0f;
    r.m[3].w = 1.0f;

    return r;
}
