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

    float minsize;

    float maxsize;

    sprite_t* character;
} scene_t;

scene_t* scene_load(const char* name, const float minsize, const float maxsize);

/**
 * The scene is responsible for releasing memory of the texture object it has
 * created for background and scale map sprites.
 */
void scene_unload(scene_t** scene);

void scene_render(scene_t* scene);
