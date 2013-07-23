/**
 * Contains the necessary structs and functions to work with sprites.
 *
 * Now it's getting interesting, this file manages the sprites that will be
 * drawn by the engine.
 *
 * You can create textures and perhaps in the future, more complex sprites
 * with just colors, lightenings, ...
 */

/**
 * @file sprite.h
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#pragma once

typedef struct sprite_gl
{

} sprite_gl;

typedef struct sprite_t
{
    /**
     * The texture associated with the sprite.
     *
     * This is mandatory in the current implementation but we could imagine
     * sprites without any texture, just colors for example.
     */
    texture_t* texture;

    /* OpenGL specifics */
    unsigned int program;

    /* Position attribute. */
    unsigned int position_attrib;

    /* TODO Move these into the texture structure or a specific program
     * structure.
     */
    unsigned int texture_attrib;
    unsigned int texture_uniform;
} sprite_t;


/**
 * Creates a new sprite and allocates the necessary memory.
 *
 * @param texture_name The name of the texture to load.
 * This means that it will call the necessary functions to load the texture as
 * well so the user does not have to do it.
 * @return A new sprite object with the memory already allocated.
 */
sprite_t* sprite_create(const char* texture_name);

/**
 * Frees the memory of a sprite.
 *
 * @param sprite The sprite to free.
 */
void sprite_delete(sprite_t** sprite);


void sprite_render(sprite_t* sprite);
