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
 * Multiplies a vector by a scalar.
 */
vec2_t vec2_mul_scal(const vec2_t a, const float s);

/**
 * Loads an 4x4 Identity matrix.
 *
 * @return The identity matrix.
 */
mat4_t mat4_identity(void);

/**
 * Creates an orthographic matrix.
 */
mat4_t mat4_ortho(const float left, const float right,
                  const float bottom, const float top,
                  const float near, const float far);

/**
 * Mutliplies a 4x4 matrix by a 2x2 vector to produce a translation matrix
 * based on vector #v.
 *
 * The 2x2 vector is actually augmented into a 3x3 vector with z = 0.
 *
 * @param m The source matrix.
 * @param v The translation vector.
 * @return The translation matrix.
 */
mat4_t mat4_translate_vec2(const mat4_t* m, const vec2_t* v);

/**
 * Mutliplies a 4x4 matrix by a 3x3 vector to produce a translation matrix
 * based on vector #v.
 *
 * @param m The source matrix.
 * @param v The translation vector.
 * @return The translation matrix.
 */
mat4_t mat4_translate_vec3(const mat4_t* m, const vec3_t* v);

/**
 * Scales a matrix according to a float that will be applied to the vectors of
 * the matrices (same scaling in every coordinate).
 *
 * @param m The source matrix.
 * @param s The scaling
 * @return The scaling matrix.
 */
mat4_t mat4_scale_1f(const mat4_t* m, const float s);
