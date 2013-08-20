/**
 * @file algebra.c
 * @author Jeremy Attali, Johan Attali
 * @date August 14, 2013
 */

#include "bina.h"

vec2_t
vec2_add(const vec2_t a, const vec2_t b)
{
    vec2_t r;

    r.x = a.x + b.x;
    r.y = a.y + b.y;

    return r;
}

vec2_t
vec2_sub(const vec2_t a, const vec2_t b)
{
    vec2_t r;

    r.x = a.x - b.x;
    r.y = a.y - b.y;

    return r;
}

float
vec2_norm(const vec2_t a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

vec2_t
vec2_normalize(const vec2_t a)
{
    vec2_t r;
    float  n = vec2_norm(a);

    r.x = a.x / n;
    r.y = a.y / n;

    return r;
}

vec2_t
vec2_mul_scal(const vec2_t a, const float s)
{
    vec2_t r;

    r.x = a.x * s;
    r.y = a.y * s;

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
mat4_ortho(const float left, const float right,
           const float bottom, const float top,
           const float near, const float far)
{
    mat4_t r;

    memset(&r, 0, sizeof(mat4_t));

    r.m[0].x = ( 2.0f / (right - left));
    r.m[1].y = ( 2.0f / (top - bottom));
    r.m[2].z = (-2.0f / (far - near));

    r.m[3].x = -(right + left) / (right - left);
    r.m[3].y = -(top + bottom) / (top - bottom);
    r.m[3].z = -(far + near) / (far - near);
    r.m[3].w = 1.0f;

    return r;
}

mat4_t
mat4_translate_vec2(const mat4_t* m, const vec2_t* v)
{
    vec3_t v3;

    v3.x = v->x;
    v3.y = v->y;
    v3.z = 0.0f;

    return mat4_translate_vec3(m, &v3);
}

mat4_t
mat4_translate_vec3(const mat4_t* m, const vec3_t* v)
{
    mat4_t r;

    float x = v->x;
    float y = v->y;
    float z = v->z;

    memcpy(&r, m, sizeof(mat4_t));

    r.m[3].x = m->m[0].x * x + m->m[1].x * y + m->m[2].x * z + m->m[3].x;
    r.m[3].y = m->m[0].y * x + m->m[1].y * y + m->m[2].y * z + m->m[3].y;
    r.m[3].z = m->m[0].z * x + m->m[1].z * y + m->m[2].z * z + m->m[3].z;
    r.m[3].w = m->m[0].w * x + m->m[1].w * y + m->m[2].w * z + m->m[3].w;

    return r;
}

mat4_t
mat4_scale_1f(const mat4_t* m, const float s)
{
    mat4_t r;
    int    i;

    memcpy(&r, m, sizeof(mat4_t));

    for (i = 0; i < 3; i++) {
        r.m[i] = vec4_scale(m->m[i], s);
    }

    /* r.m[3].x = 0.0f; */
    /* r.m[3].y = 0.0f; */
    /* r.m[3].z = 0.0f; */
    /* r.m[3].w = 1.0f; */

    return r;
}

lin_vec2_anim_t
lin_vec2_anim_create(vec2_t from, vec2_t to, float speed, float elapsed)
{
    lin_vec2_anim_t r;

    unsigned int steps = 1; /* Number of steps */
    float norm = 0.0f;      /* || to - from || */

    if (elapsed <= 0) {
        LOGE(BINA_INVALID_PARAM);
    }

    norm  = sqrtf(powf(to.x - from.x, 2.0f) + powf(to.y - from.y, 2.0f));
    steps = (int) (norm * speed * 1.0f / elapsed);

    r.steps    = steps;
    r.step     = 0;
    r.from     = from;
    r.to       = to;
    r.offset.x = (to.x - from.x) / steps;
    r.offset.y = (to.y - from.y) / steps;

    return r;
}
