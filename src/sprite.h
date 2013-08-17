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

typedef enum sprite_cam_type_e
{
    /* SPRITE_CAM_TYPE_BACK, */
    SPRITE_CAM_TYPE_FRONT,
    /* SPRITE_CAM_TYPE_RIGHT, */
    /* SPRITE_CAM_TYPE_LEFT, */
    SPRITE_CAM_TYPE_COUNT   /* trick to get the number of elements */
} sprite_cam_type_e;

typedef enum sprite_anim_e
{
    SPRITE_ANIM_NEUTRALPOSE,
    SPRITE_ANIM_STOPACTION1,
    /* SPRITE_ANIM_WALKCYCLE, */
    SPRITE_ANIM_COUNT        /* trick to get the number of elements */
} sprite_anim_e;

typedef enum sprite_anim_status_e
{
    SPRITE_ANIM_STATUS_PLAYING = 0,
    SPRITE_ANIM_STATUS_LINEAR_FINISHED = 1,
    SPRITE_ANIM_STATUS_TEXTURE_FINISHED = 2,
    SPRITE_ANIM_STATUS_DONE = 4,
    SPRITE_ANIM_STATUS_INVALID = -1
} sprite_anim_status_e;

typedef struct sprite_tex_anim_t
{
    /**
     * Total number of animations held by #list.
     *
     * This is the product: SPRITE_ANIM_COUNT * SPRITE_CAM_TYPE_COUNT
     */
    unsigned int size;

    unsigned short nb_cam_types;

    unsigned short nb_anims;

    sprite_cam_type_e cur_cam_type;

    sprite_anim_e cur_anim;

    texture_list_t** list;

} sprite_tex_anim_t;

/**
 * A simple animator for sprites.
 *
 * Right now it only allows to update positions based on linear updates.
 * This could be improved a lot by adding rotations, scaling, ...
 */
typedef struct sprite_animator_t
{
    /**
     * Animations have ids to be managed later by real animators.
     */
    unsigned long id;

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
     * Linear animation (optional).
     *
     * Can be set to NULL if there is only animation in the textures, no
     * movement implied.
     */
    lin_vec2_anim_t* animation;

    /**
     * The list of textures to use during sprite animation (optional).
     *
     * Can be set to NULL if there is no switching between textures.
     */
    texture_list_t* textures;

} sprite_animator_t;

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

    /* TODO Move all these OpenGL attributes into the texture structure or
     * a specific program structure in #shader.c.
     */
    int texture_attrib;
    int texture_uniform;
    int mvp_uniform;

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
     * Barycenter of the sprite.
     *
     * A sprite can also optionally have an center. This represents the center
     * of gravity of the sprite. Think of it as the center of the sprite
     * relative to the object size.
     */
    vec2_t center;

    /**
     * Scale of the sprite.
     *
     * This is used in order to make a sprite bigger or smaller on the
     * viewport.
     */
    float scale;

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
     * Default: 2.0f;
     */
    float width;

    /**
     * The height of the sprite.
     *
     * Can vary from the texture itself because the sprite is represented in
     * the projection matrix system. (Default: -1,-1 -> +1,+1).
     *
     * Default: 2.0f;
     */
    float height;

    /**
     * Matrix associated with the sprite.
     *
     * It contains the multiplications of:
     * matrix = PROJECTION * VIEW * MODEL;
     */
    mat4_t mvp;

    /**
     * All the different texture animations of the sprite.
     *
     * For example: camRight -> neutralPose, ...
     * This can probably be moved to a more specific type of sprites (like
     * character).
     */
    sprite_tex_anim_t* tex_anims;

    /**
     * Current animator of the sprite.
     *
     * Can be NULL if there is no animation on the sprite.
     * Can be used to:
     *  - Animate the sprite accross space (linear animation).
     *  - Animate the textures of the sprite (make it live!).
     *  - Do both at the same time.
     */
    sprite_animator_t* animator;

} sprite_t;

/**
 * Creates a new sprite and allocates the necessary memory.
 *
 * Note that a sprite does not have a size yet. This is because currently,
 * Yaum renders every object at full resolution (same as background). This
 * makes things easier for everyone but is a lot more heavy on resources as
 * even a tiny sprite will be rendered at Full HD.
 *
 * @param texture The texture associate with the sprite. Can be NULL.
 * @param position The original position of the sprite.
 * @param center The offset that represents the actual center of the object.
 * This is like the 0, 0 of the object in its coordinate system.
 * @param size The size of the scene. Sprite will be full sized. See comment
 * above.
 * @param scale The initial scale of the sprite. Can be altered later via
 * animation. This is what the scale map is for.
 * @return A new sprite object with the memory already allocated.
 */
sprite_t* sprite_create(texture_t* texture,
                        const unsigned int program,
                        const vec2_t position,
                        const vec2_t offset,
                        const vec2_t size,
                        const float  scale);

sprite_t* sprite_load_character(const char* name,
                                const unsigned int program,
                                const vec2_t position,
                                const vec2_t offset,
                                const vec2_t size,
                                const float  scale);

/**
 * Frees the memory of a sprite.
 *
 * Note that the sprite should not be responsible for deleting the texture.
 * The texture should be deleted by the object that created it.
 * @param sprite The sprite to free.
 */
void sprite_delete(sprite_t** sprite);

/**
 * Sets the scale factor of a sprite.
 *
 * @param srite The sprite to set the ratio.
 * @param scale The scale of the sprite.
 */
void sprite_set_position(sprite_t* const sprite, const vec2_t* position);

/**
 * Sets the scale factor of a sprite.
 *
 * @param srite The sprite to set the ratio.
 * @param scale The scale of the sprite.
 */
void sprite_set_scale(sprite_t* const sprite, const float scale);

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
 * Sets the sprite to given camera type.
 *
 * @param sprite The sprite to set.
 * @param cam The camera to set the sprite to.
 */
void sprite_set_cam_type(sprite_t* sprite, sprite_cam_type_e cam);

/**
 * Sets the animation of the sprite.
 *
 * This puts the animation at key: 0.
 *
 * @param sprite The sprite to set.
 * @param anim The animation to set the sprite to.
 */
void sprite_set_animation(sprite_t* sprite, sprite_anim_e anim);

/*
 * Computes the Model View Projection matrix of a sprite.
 *
 * This function will set the appropriate MVP matrix to the sprite.
 *
 * @param sprite The sprite.
 */
void sprite_compute_mvp(sprite_t* sprite);

/**
 * Animates the character to a given position.
 *
 * @param sprite The sprite to animate (which holds its current position).
 * @param to The destination of the sprite.
 * @param speed The speed of the sprite.
 * @param elapsed Time elapsed since last frame. TODO Get rid of me.
 */
void sprite_animate_char_to(sprite_t* sprite, vec2_t to, float speed,
                            float elapsed);

/**
 * Animates the sprite into 'idle' mode.
 *
 * Randomly selects an idle animation and sets the sprite to it.
 *
 * @param sprite The sprite to animate.
 */
void sprite_animate_idle(sprite_t* sprite);

/**
 * Animates the sprite if there is an animator linked to it.
 *
 * @param sprite The sprite to animate.
 * @param elapsed The time elapsed since last frame.
 */
sprite_anim_status_e sprite_animate(sprite_t* sprite, float elapsed);
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
 * @param animation The linear animation that will be used to animate the
 * sprite. Can be NULL if we are only switching textures.
 * @param textures The texture list to use during animation. Can be NULL if
 * there is no switching.
 * @return The animator with proper memory allocated. You need to call
 * #sprite_animator_delete to clean up the memory used by the structure.
 */
sprite_animator_t* sprite_animator_create(sprite_t* sprite,
                                          lin_vec2_anim_t* animation,
                                          texture_list_t* textures);

/**
 * Frees memory allocated by #sprite_animator_create.
 *
 * The function is responsible for deleting the linear animation because it
 * will not be used anymore. On the contrary, it should not remove the texture
 * list object because it has been somewhere else and will be reused.
 *
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
