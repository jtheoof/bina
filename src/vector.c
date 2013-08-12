/**
 * @file vector.c
 * @author Jeremy Attali, Johan Attali
 * @date July 24, 2013
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

