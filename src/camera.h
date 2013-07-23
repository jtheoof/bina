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
 * Sets camera viewport for use with OpenGL.
 *
 * @param x,y Specify the lower left corner of the viewport rectangle, in
 * pixels. The initial value is (0,0).
 * @param width, height Specify the width and height of the viewport. When
 * a GL context is first attached to a window, width and height are set to the
 * dimensions of that window.
 */
void camera_set_viewport(int x, int y, int width, int height);

/**
 * Supposed to set the orthographic camera.
 *
 * <strong>NOT YET IMPLEMENTED ON DEVICES.</strong>
 */
void camera_set_ortho(int width, int height);
