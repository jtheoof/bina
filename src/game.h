/**
 * Game header file.
 */

/**
 * @file game.h
 * @author Jeremy Attali, Johan Attali
 * @date August 20, 2013
 */

#pragma once

#include "algebra.h"
#include "scene.h"

#define GAME_WIDTH        960
#define GAME_HEIGHT       540
#define GAME_REFRESH_RATE 16  /* 60 times per second */

typedef struct game_t
{
    /**
     * Current scene loaded and present on the screen.
     *
     * A scene, for now, is composed of the background image and an associated
     * scale map which serves to compute the scale of object depending on its
     * position on the image.
     */
    scene_t* scene;

} game_t;

/**
 * Initalization of program.
 *
 * Loads assets from disk or memory and prepares the rendering engine.
 * @param width The width of the viewport.
 * @param height The height of the viewport.
 */
void game_init(game_t* game, int width, int height);

/**
 * Frees objects.
 *
 * Should be called when program exits.
 * Perhaps code or callback can vary depending on devices.
 */
void game_end(game_t* game);

/**
 * Renders game.
 */
void game_render();

/**
 * Animate porc on the screen when clicking or tapping.
 *
 * This is just a testing function. The goal is to make it work on every
 * device.
 * @param to The destination where porc should go.
 * @param speed The speed we want to set porc to.
 */
void game_animate_porc_to(scene_t* scene, vec2_t to, float speed);
