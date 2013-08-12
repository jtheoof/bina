/**
 * Contains the necessary structs and functions to deal with math. 
 * 
 * Vectors, matrices, ...
 */

/**
 * @file vector.h
 * @author Jeremy Attali, Johan Attali
 * @date July 24, 2013
 */

#pragma once

typedef struct vec2_t
{
    float x;
    float y;
} vec2_t;

typedef struct vec3_t
{
    float x;
    float y;
    float z;
} vec3_t;

/**
 * Substracts two vectors.
 */
vec2_t vec2_sub(const vec2_t a, const vec2_t b);

/* static vec2_t */
/* vec2_add(const vec2_t a, const vec2_t b) */
/* { */
/*     vec2_t r; */

/*     r.x = a.x + b.x; */
/*     r.y = a.y + b.y; */

/*     return r; */
/* } */
