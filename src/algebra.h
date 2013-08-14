/**
 * Contains the necessary structs and functions to deal with linear algebra.
 *
 * In particular, this is where we find our vectors and matrices structures
 * and functions.
 */

/**
 * @file algebra.h
 * @author Jeremy Attali, Johan Attali
 * @date August 14, 2013
 */

#pragma once

#ifndef PI
#define PI 3.14159265359f
#endif

#define DEG_TO_RAD  PI / 180.0f
#define RAD_TO_DEG  90.0f / PI

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

typedef struct vec4_t
{
    float x;
    float y;
    float z;
    float w;
} vec4_t;

typedef struct mat2_t
{
    vec2_t m[2];
} mat2_t;

typedef struct mat3_t
{
    vec3_t m[3];
} mat3_t;

typedef struct mat4_t
{
    vec4_t m[4];
} mat4_t;

/**
 * Adds two vectors.
 */
vec2_t vec2_add(const vec2_t a, const vec2_t b);

/**
 * Substracts two vectors.
 */
vec2_t vec2_sub(const vec2_t a, const vec2_t b);

/**
 * Loads an 4x4 Identity matrix.
 *
 * @return The identity matrix.
 */
mat4_t mat4_identity(void);

/**
 * Scales a matrix according to x, y, z floats.
 */
mat4_t mat4_scale_1f(const mat4_t* m, const float s);
