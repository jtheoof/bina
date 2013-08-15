/**
 * Contains the rendering functions.
 *
 * Should be almost completely agnostic of the differences between OpenGL and
 * OpenGL ES. Retrieving time spent and elapsed is platform dependent for now.
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
void renderer_init(const camera_viewport_t* viewport, const mat4_t* projection);

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
