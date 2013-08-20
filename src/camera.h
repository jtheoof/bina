/**
 * Contains the necessary structs and functions to deal with the camera. 
 *
 * Right now it does not do much because we are just implemeting a few tries.
 * Later it should at least support camera ortho for vertex buffer
 * manipulation through MODELVIEWPROJECTION matrix.
 */

/**
 * @file camera.h
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#pragma once

/**
 * Basic info on the viewport used for the camera (window coordinates).
 */
typedef struct camera_win_info_t
{
    /**
     * The x coordinate of the viewport.
     */
    int x;

    /**
     * The y coordinate of the viewport.
     */
    int y;

    /**
     * The width of the viewport.
     */
    int width;

    /**
     * The height of the viewport;
     */
    int height;

} camera_win_info_t;

/**
 * Basic info on the projection matrix used for the camera (eye coordinates).
 */
typedef struct camera_eye_info_t
{
    /**
     * Scaling vector present in the projection matrix.
     */
    vec3_t scale;

    /**
     * Translation vector present in the projection matrix.
     */
    vec3_t trans;

} camera_eye_info_t;

typedef struct camera_t
{
    camera_win_info_t viewport;

    camera_eye_info_t eye;

    mat4_t projection;
} camera_t;

/**
 * Sets camera viewport for use with OpenGL.
 *
 * @param viewport The viewport to set by OpenGL.
 * Should be called when window that draws OpenGL changes size.
 */
void camera_set_viewport(const camera_win_info_t* viewport);

/**
 * Retrieves the current projection matrix.
 *
 * @return A 4x4 matrix which represents the current projection matrix.
 */
mat4_t camera_get_projection();

/**
 * Sets the projection matrix.
 *
 * @param projection The new projection matrix.
 */
void camera_set_projection(const mat4_t* projection);

/**
 * Normalizes a point to the screen into a vector with a norm ranging from 0,
 * to 1 (Window Coordinates to Normalized Device Coordinates).
 *
 * For example if the viewport is Full HD and the point represents the bottom
 * right of the screen, this function will return a vector with coordinate:
 * [1, 1].
 */
vec2_t camera_win_coord_to_ndc(const vec2_t* point);

/**
 * Converts window coordinates to eye coordinates in the current projection
 * matrix.
 *
 * For now the project matrix is the default one.
 * Example of window coordinates: 1920, 1080
 * Example of corresponding eye coordinates: 1, -1 (default projection).
 *
 * In OpenGL, initially the object and camera frames are the same.
 * - Default model-view matrix is an identity The camera is located at origin
 *   and points in the negative z direction OpenGL also specifies a default
 *   view volume that is a cube with sides of length 2 centered at the origin
 * - Default projection matrix is an identity
 * @param point The coordinates in current screen space.
 * @return The computed coordinates.
 */
vec2_t camera_win_coord_to_eye(const vec2_t* point);

/**
 * Used to compute the absolute (normalized) point from a projection point.
 * For example in the default projection matrix the point (0, 0) is (1/2, 1/2)
 * in normalized coordinates.
 *
 * @param point The point in Eye coordinates.
 */
vec2_t camera_eye_coord_to_ndc(const vec2_t point);
