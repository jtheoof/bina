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

typedef struct camera_viewport_t
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
} camera_viewport_t;

/**
 * Sets camera viewport for use with OpenGL.
 *
 * @param viewport The viewport to set by OpenGL.
 * Should be called when window that draws OpenGL changes size.
 */
void camera_set_viewport(camera_viewport_t* viewport);

/**
 * Converts screen coordinates to coordinates in the current projection
 * matrix.
 *
 * For now the project matrix is the default one.
 *
 * In OpenGL, initially the object and camera frames are the same.
 * - Default model-view matrix is an identity The camera is located at origin
 *   and points in the negative z direction OpenGL also specifies a default
 *   view volume that is a cube with sides of length 2 centered at the origin
 * - Default projection matrix is an identity
 * @param x The x coordinate in current screen space.
 * @param y The y coordinate in current screen space.
 * @param coord The computed coordinates.
 */
void camera_screen_coord_to_proj(const int x, const int y, vec2_t* coord);

/**
 * Retrieves OpenGL viewport object.
 *
 * @param viewport A GLint[4] array
 */
void camera_get_viewport(int* viewport);
