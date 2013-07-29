/**
 * Contains the rendering functions.
 *
 * Should be almost completely agnostic of the differences between OpenGL and
 * OpenGL ES.
 */

/**
 * @file renderer.h
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#pragma once

/**
 * Initializes the render with viewport dimensions specified.
 */
void renderer_init(camera_viewport_t* viewport);

/**
 * Pre-rendering function.
 *
 * Only clears the buffer for now.
 * @param r, g, b, a RGBA values of clear color.
 */
void renderer_pre_render(float r, float g, float b, float a);

/**
 * Main renderer function that is currently called by devices and oses from
 * custom loop game places.
 *
 * This where the common rendering parts comes into place.
 */
void renderer_render();

/**
 * Retrieves time.
 *
 * Could be time elapsed since epoch or time elapsed since programme started
 * depending on CLOCK implementation. See man clock_gettime.
 *
 * @return Time elapsed in nanoseconds.
 */
int64_t renderer_get_time_nano();

/**
 * Returns the time elapsed in ms since last render.
 *
 * The implementation of this function can vary depending on the the device.
 * For example GLUT has a very easy way to set it up.
 *
 * @return The elapsed time in ms.
 */
float renderer_get_time_elapsed();
