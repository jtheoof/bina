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

/**
 * A simple animator for sprites.
 *
 * Right now it only allows to update positions based on linear updates.
 * This could be improved a lot by adding rotations, scaling, ...
 */
typedef struct sprite_animator_t
{
    /**
     * How many steps are required in the animation.
     */
    unsigned int steps;

    /**
     * The current step we are in while animating the sprite.
     */
    unsigned int step;

    /**
     * The offset we need to add to current position on each update.
     */
    vec2_t offset;
} sprite_animator_t;

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

    /**
     * The position of the left-bottom coordinate of the sprite.
     *
     * Later we can had more like "pivot point" to characterize a bit more the
     * sprite. This can be useful if the sprite should have some sort of
     * barycenter for the example the feet of Brian are not at the left-bottom
     * in the picture itself.
     */
    vec2_t position;

    /**
     * The OpenGL uniform of the position, used by the vertex shader.
     *
     * We use a uniform because the position of the sprite could vary. The
     * uniform allows us to change a vertex buffer variable from the C code
     * (CPU memory).
     */
    unsigned int position_uniform;

    float vertices[4][2];

    /**
     * The width of the sprite.
     *
     * Can vary from the texture itself because the sprite is represented in
     * the projection matrix system. (Default: -1,-1 -> +1,+1).
     *
     * Default: 1.0f;
     */
    float width;

    /**
     * The height of the sprite.
     *
     * Can vary from the texture itself because the sprite is represented in
     * the projection matrix system. (Default: -1,-1 -> +1,+1).
     */
    float height;
} sprite_t;


/**
 * Creates a new sprite and allocates the necessary memory.
 *
 * @param texture_name The name of the texture to load.
 * This means that it will call the necessary functions to load the texture as
 * well so the user does not have to do it.
 * @return A new sprite object with the memory already allocated.
 */
sprite_t* sprite_create(const char* texture_name, vec2_t position,
                        float width, float height);

/**
 * Frees the memory of a sprite.
 *
 * @param sprite The sprite to free.
 */
void sprite_delete(sprite_t** sprite);


/**
 * Renders a sprite to the screen.
 *
 * Proper calls to #sprite_create should have been made.
 * OpenGL needs to be set up properly (optinally with texturing).
 */
void sprite_render(sprite_t* sprite);

/**
 * Creates a new animator object.
 *
 * An animator object only contains linear positions from current position
 * to end position (with number of steps and current step).
 * This is very simple for now, we'll make it more complex later.
 *
 * @param sprite The original sprite we want to animate. It contains the
 * original position.
 * @param to The end position. Where the sprite should end up.
 * @param speed The speed at which the sprite should be moving.
 * The speed is taken into account when computing the steps.
 * @elapsed The time elapsed since last frame. It used to get an idea of how
 * far the renderer is going. Right now the implementation if too basic, for
 * example if last frame was very slow for some reason then the number of
 * steps will be very short even if GPU rendering comes back up right after.
 * @return The animator with proper memory allocated. You need to call
 * #sprite_animator_delete to clean up the memory used by the structure.
 */
sprite_animator_t* sprite_animator_create(sprite_t* sprite, vec2_t to,
                                          float speed, float elapsed);

/**
 * Frees memory allocated by #sprite_animator_create.
 * @param animator The animator to free.
 */
void sprite_animator_delete(sprite_animator_t** animator);

/**
 * Animates the sprite given an animator.
 *
 * This function will increase the current step of animator object.
 * The time elapsed is present in order to avoid different speed time with
 * different graphics performance. We base the initial speed of the sprite on
 * a 60 FPS rendering throughput (0.016s between each frame).
 *
 * @param sprite The sprite to animate.
 * @param animator The animator previously created.
 * @param elapsed The time elapsed in ms since last frame.
 * @return The number of steps left. When this function returns 0, it is
 * mandatory to clean up the memory and free animator object through
 * #sprite_animator_delete.
 */
unsigned int sprite_animator_animate(sprite_t* sprite,
                                     sprite_animator_t* animator,
                                     float elapsed);
