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

typedef struct sprite_t
{
    /**
     * The texture associated with the sprite.
     *
     * This is mandatory in the current implementation but we could imagine
     * sprites without any texture, just colors for example.
     * The nice thing about this current implementation is that the pointer to
     * the #texture_t object can vary and this can be used to render different
     * animations.
     */
    texture_t* texture;

    /* OpenGL program previously created by #shader_create_program. */
    unsigned int program;

    /* Position attribute. */
    int position_attrib;

    /* TODO Move these into the texture structure or a specific program
     * structure.
     */
    int texture_attrib;
    int texture_uniform;

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
    int position_uniform;

    /**
     *
     * The OpenGL uniform of the scaling, used by the vertex shader.
     *
     * Not all sprites have this uniform. Check their vertex shader to know
     * for sure.
     */
    int scaling_uniform;

    /**
     * Vertex positions of the sprite.
     *
     * Right now it's only stored for Vertex Buffer attribute.
     */
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
     *
     * Default: 1.0f;
     */
    float height;

} sprite_t;

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
     * Initial elapsed time.
     *
     * Right now, animators are created with the current elapsed time when
     * call executes the code. This is done to compute the number of steps
     * required to make the move from point A to point B. There is for sure
     * a much better way to do it. But for now it works.
     * This is used when animating the sprite later so that the offset can be
     * multplied by the right amont. ielapsed stands for initial_elapsed.
     */
    float ielapsed;

    /**
     * Time elapsed in seconds since the animation has started.
     */
    float elapsed;

    /**
     * The offset we need to add to current position on each update.
     */
    vec2_t offset;

    /**
     * The list of textures to use during sprite animation.
     *
     * Can be set to NULL if there is no switching between textures.
     */
    texture_list_t* textures;

} sprite_animator_t;

/**
 * Creates a new sprite and allocates the necessary memory.
 *
 * @param texture The texture associate with the sprite. Can be NULL.
 * @position The original position of the sprite.
 * @offset The offset that represents the actual center of the object.
 * This is like the 0, 0 of the object in its coordinate system.
 * @size The size in x (width) and y (height) of the sprite.
 * @return A new sprite object with the memory already allocated.
 */
sprite_t* sprite_create(texture_t* texture,
                        const unsigned int program,
                        const vec2_t position,
                        const vec2_t offset,
                        const vec2_t size);

/**
 * Frees the memory of a sprite.
 *
 * Note that the sprite should not be responsible for deleting the texture.
 * The texture should be deleted by the object that created it.
 * @param sprite The sprite to free.
 */
void sprite_delete(sprite_t** sprite);

/**
 * Updates a sprite with a new texture previously created.
 *
 * This method is useful when animating a sprite and updating its texture at
 * the same time. It's more useful to call this function before #sprite_render
 * because #sprite_render will use sprite->texture to render the right
 * texture.
 *
 * @param sprite The sprite to update.
 * @param texture The pointer to the new texture.
 */
void sprite_set_texture(sprite_t* sprite, texture_t* texture);

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
 * @param tex The texture list to use during animation. Can be NULL if there
 * is no switching.
 * @param to The end position. Where the sprite should end up.
 * @param speed The speed at which the sprite should be moving.
 * The speed is taken into account when computing the steps.
 * @elap The time elapsed since last frame. It used to get an idea of how
 * far the renderer is going. Right now the implementation if too basic, for
 * example if last frame was very slow for some reason then the number of
 * steps will be very short even if GPU rendering comes back up right after.
 * @return The animator with proper memory allocated. You need to call
 * #sprite_animator_delete to clean up the memory used by the structure.
 */
sprite_animator_t* sprite_animator_create(sprite_t* sprite, texture_list_t* tex,
                                          vec2_t to, float speed, float elap);

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
 * If there is a texture list associated with the animator, we also update the
 * current texture based on Yaum's real time rendering (24 FPS).
 *
 * @param sprite The sprite to animate.
 * @param animator The animator previously created.
 * @param elapsed The time elapsed in ms since last frame.
 * @return The number of steps left. When this function returns 0, it means
 * that there is no more step to animate. But there could be a texturing
 * animation to finish. We return -1 if, in addition to that, there is no more
 * texture to animate (we have reached the end of the sequence). It is
 * mandatory to clean up the memory and free animator object through
 * #sprite_animator_delete.
 */
int sprite_animator_animate(sprite_t* sprite, sprite_animator_t* animator,
                            float elapsed);
