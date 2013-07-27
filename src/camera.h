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
 * Supposed to set the orthographic camera.
 *
 * <strong>NOT YET IMPLEMENTED ON DEVICES.</strong>
 */
void camera_set_ortho(int width, int height);
