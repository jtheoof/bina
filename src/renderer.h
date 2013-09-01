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

#include "scene.h"

/**
 * Initializes the render with viewport dimensions specified.
 */
void renderer_init();

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
 *
 * @param game The game to render.
 */
void renderer_render(scene_t* scene);

/**
 * Checks an extension in the list of GL_EXTENSIONS.
 *
 * The checking is done directly on the hardware.
 *
 * @param ext The extension to check.
 * @return A number different from 0 if extension is found, 0 if not.
 */
short renderer_has_gl_ext(const char* ext);

/**
 * Retrieves texture compression extension specific to hardware running.
 *
 * @return The extension based on hardware capabilites. For example it could
 * be 'dds', 'ktx', ...
 */
char* renderer_get_tc_ext();
