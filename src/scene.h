/**
 * Contains all the necessary structs and functions to deal with a scene.
 *
 * A scene, for now, is composed of the background image and an associated
 * scale map which serves to compute the scale of object depending on its
 * position on the image. The scale map is, of course, not visible on the
 * screen. It is simply used to scale an object according to the pixel color.
 */

/**
 * @file scene.h
 * @author Jeremy Attali, Johan Attali
 * @date August 10, 2013
 */

#pragma once

#include "sprite.h"

/* TODO Remove this if unsused. */
typedef struct scene_char_t
{
    float minsize;

    float maxsize;

    sprite_t* sprite;
} scene_char_t;

typedef struct scene_t
{
    /**
     * The background of the scene.
     *
     * For now we do not support scenes than spans past the viewport (scenes
     * too large for the camera).
     */
    sprite_t* background;

    /**
     * The scale map.
     *
     * A white pixel corresponds to #maxsize for character. A black pixel
     * corresponds to #minsize for character. This does not need to be
     * a sprite because it will not be drawn on the screen. It will just be
     * used to compute the final size of a sprite (ex: the main character).
     */
    texture_t* smap;

    /**
     * Background program id.
     *
     * Obtained via #shader_create_program.
     */
    unsigned int bg_prog;

    /**
     * Character program id.
     *
     * Obtained via #shader_create_program.
     */
    unsigned int ch_prog;

    /**
     * Minimum scale of the character.
     *
     * This is a normalized value. For example 0.5f will mean that the minimum
     * size of the character will be half of the original one.
     */
    float scale_min;

    /**
     * Maximum scale of the character.
     *
     * This is a normalized value. For example 1.0f will mean that the maximum
     * size of the character will be the original one.
     */
    float scale_max;

    /**
     * The difference between #scale_max and #scale_min.
     *
     * This is used in order to avoid doing it many times.
     */
    float scale_dif;

    /**
     * The character that moves around the scene.
     */
    sprite_t* character;

    /**
     * The width of the scene.
     *
     * Used to render the viewport and the background for example.
     */
    unsigned int width;

    /*
     * The height of the scene.
     *
     * Used to render the viewport and the background for example.
     */
    unsigned int height;

    /**
     * Time since the character has been idle (no animation applied to it).
     */
    float time_idle;

    /**
     * A simple flag to indicate that the scene has been loaded and is ready
     * to be rendered.
     */
    short is_ready;

} scene_t;

/**
 * Loads a scene with a name.
 *
 * The scene also specifies the minimum scale and maximum scale of the
 * character.
 *
 * Later we can imagine having a real manifest for a scene. With all
 * objects presents, their positions, sizes, scales, ...
 *
 * @param name The name of the scene. Must be the name of the folder present
 * in 'scenes' assets. The folder must contain a 'background.png' and
 * 'scaleMap.png' file.
 * @param minsize The minimum scaling ratio of the character.
 * @param maxsize The maximum scaling ratio of the character.
 * @return The scene created.
 */
scene_t* scene_load(const char* name,
                    const float minsize, const float maxsize);

/**
 * The scene is responsible for releasing memory of the texture object it has
 * created for background and scale map sprites.
 *
 * @param scene The scene to delete.
 */
void scene_unload(scene_t** scene);

/**
 * Animates the scene.
 *
 * @param scene The scene to animate.
 * @param elapsed Time elapsed since last frame.
 */
void scene_animate(scene_t* scene, float elapsed);

/**
 * Renders the scene.
 *
 * @param scene The scene to render.
 */
void scene_render(scene_t* scene);

/**
 * Moves the character in the scene.
 *
 * @param scene The scene where the character is present.
 * @param screen The destination where the character should go in screen
 * coordinates.
 * @param speed The speed we want to set it to.
 */
void scene_move_character_to(scene_t* scene, vec2_t screen, float speed);

/**
 * Computes the size of the character based on its position thanks to the
 * scale map.
 *
 * @param scene The scene where the character is.
 * @param norm A normalized point (0 to 1) that represents the coordinate we
 * are looking for in the scale map.
 * @return A ratio between 0 and 1 that corresponds to the pixel size.
 * 0 stands for minimum size in the scene. 1 stands for maximum size in the
 * scene.
 */
float scene_compute_character_size(scene_t* scene, const vec2_t norm);
