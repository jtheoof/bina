/**
 * @file sprite.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

/* XXX This should actually be comming from some sort of scene/manifest
 * loader.
 * TODO remove me when proper scene management has been developped.
 */
static const token_string_t g_cam_type_names[] = {
    { SPRITE_CAM_TYPE_FRONT, "frontCam" },
};

static const token_string_size_t g_anim_names_sizes[] = {
    { SPRITE_ANIM_NEUTRALPOSE, "neutralPose", 30 },
};

/**
 * Constant holding default values for texices.
 *
 * This will probably always be the same, so keeping it should make sense.
 */
static const float sprite_texices_g[] = {
     0.0f,  1.0f,
     0.0f,  0.0f,
     1.0f,  1.0f,
     1.0f,  0.0f
};

static sprite_anim_t*
create_character_anim(const char* character)
{
    sprite_anim_t*   ret  = NULL;
    texture_list_t** list = NULL;
    int i, c, a, size;
    char filename[MAX_PATH];

    int cams  = SPRITE_CAM_TYPE_COUNT;
    int anims = SPRITE_ANIM_COUNT;

    ret = (sprite_anim_t*) malloc(sizeof(sprite_anim_t));

    if (!ret) {
        LOGE("BINA_NOT_ENOUGH_MEMORY");
        goto error;
    }

    LOGD("number of camera types: %d", SPRITE_CAM_TYPE_COUNT);
    LOGD("number of sprite animations: %d", SPRITE_ANIM_COUNT);

    list = (texture_list_t**) calloc(cams * anims, sizeof(texture_list_t*));

    if (!list) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

    /* Load the animation for each camera type and animation name */
    for (c = 0; c < cams; c++) {
        for (a = 0, i = c * cams + a; a < anims; a++) {
            snprintf(filename, MAX_PATH, "%s_%s_camSetup_%s", character,
                     g_anim_names_sizes[a].string, g_cam_type_names[c].string);
            size = g_anim_names_sizes[a].size;
            list[i] = texture_create_list(character, filename, "png", size);
        }
    }

    ret->list = list;
    ret->nb_cam_types = SPRITE_CAM_TYPE_COUNT;
    ret->nb_anims = SPRITE_ANIM_COUNT;
    ret->size = cams * anims;

    return ret;

error:
    if (ret) {
        free(ret);
        ret = NULL;
    }

    LOGE("unable to create animations for: %s", character);
    return NULL;
}

sprite_t*
sprite_create(texture_t* texture,
              const unsigned int program, const vec2_t position,
              const vec2_t center, const vec2_t size, const float scale)
{
    sprite_t* sprite;
    float width, height;

    sprite = (sprite_t*) malloc(sizeof(sprite_t));

    if (!sprite) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        return NULL;
    }

    sprite->program = program;

    if (program) {
        sprite->mvp_uniform      = glGetUniformLocation(program, "mvp_u");
        sprite->position_attrib  = glGetAttribLocation(program, "position_a");
        sprite->texture_attrib   = glGetAttribLocation(program, "texture_a");
        sprite->texture_uniform  = glGetUniformLocation(program, "texture_u");

        LOGD("In program: %d, "
             "[mvp]: uniform: %d, "
             "[position]: attribute: %d "
             "[texture]: uniform: %d, attribute: %d",
             program,
             sprite->mvp_uniform,
             sprite->position_attrib,
             sprite->texture_uniform,  sprite->texture_attrib);
    }

    width  = size.x;
    height = size.y;

    sprite->vertices[0][0] = -2.0f / width;
    sprite->vertices[0][1] =  2.0f / height;
    sprite->vertices[1][0] = -2.0f / width;
    sprite->vertices[1][1] = -2.0f / height;
    sprite->vertices[2][0] =  2.0f / width;
    sprite->vertices[2][1] =  2.0f / height;
    sprite->vertices[3][0] =  2.0f / width;
    sprite->vertices[3][1] = -2.0f / height;

    sprite->position   = position;
    sprite->center     = center;
    sprite->scale      = scale;
    sprite->width      = width;
    sprite->height     = height;
    sprite->texture    = texture;
    sprite->mvp        = mat4_identity();
    sprite->animations = NULL;

    return sprite;
}

sprite_t*
sprite_load_character(const char* name,
                      const unsigned int program, const vec2_t position,
                      const vec2_t center, const vec2_t size,
                      const float scale)
{
    sprite_t*      sprite = NULL;
    sprite = sprite_create(NULL, program, position, center, size, scale);

    if (!sprite) {
        return NULL;
    }

    sprite->animations = create_character_anim(name);

    if (sprite->animations) {
        sprite->animations->cur_cam_type = SPRITE_CAM_TYPE_FRONT;
        sprite->animations->cur_anim = SPRITE_ANIM_NEUTRALPOSE;

        if (sprite->animations->list) {
            sprite->texture = sprite->animations->list[0]->textures[0];
        }
    }

    return sprite;
}

void
sprite_delete(sprite_t** sprite)
{
    sprite_t* s = *sprite;
    sprite_anim_t* a;

    int i ;

    /* XXX The sprite should not be responsible for deleting the texture.
     * The texture should be deleted by the object that created it.
     */
    if (s) {
        shader_delete_program(s->program);

        if (s->animations) {
            a = s->animations;

            if (a->list) {
                for (i = 0; i < a->size; i++) {
                    /* Not sure I need the () around a->list[i] */
                    texture_delete_list(&(a->list[i]));
                }
                free(a->list);
                a->list = NULL;
            }
            free(a);
            s->animations = NULL;
        }

        free(s);
        s = NULL;
    }
    *sprite = s;
}

void
sprite_set_position(sprite_t* const sprite, const vec2_t* position)
{
    if (sprite && position) {
        sprite->position = *position;
    }
}

void
sprite_set_scale(sprite_t* const sprite, const float scale)
{
    if (sprite) {
        sprite->scale = scale;
    }
}

void
sprite_set_texture(sprite_t* sprite, texture_t* texture)
{
    if (sprite && texture) {
        sprite->texture = texture;
    }
}

void
sprite_set_cam_type(sprite_t* sprite, sprite_cam_type_e cam)
{
    if (sprite && sprite->animations) {
        sprite->animations->cur_cam_type = cam;
    }
}

void sprite_set_animation(sprite_t* sprite, sprite_anim_e anim)
{

}

void
sprite_compute_mvp(sprite_t* sprite)
{
    mat4_t matrix;
    mat4_t proj = camera_get_projection();
    vec2_t translation;

    if (!sprite) {
        return;
    }

    /* Firstly, we need to recompute the center of the sprite based on the
     * current scaling of the object. If the object is half as big as its
     * original state, its center is also half of what it used to be.
     */
    translation = vec2_mul_scal(sprite->center, sprite->scale);

    /* Secondly, we move the sprite according to its current position in the
     * world.
     */
    translation = vec2_add(sprite->position, translation);
    matrix = mat4_translate_vec2(&proj, &translation);

    /* Finally, we scale everything. */
    matrix = mat4_scale_1f(&matrix, sprite->scale);

    sprite->mvp = matrix;
}

void
sprite_render(sprite_t* sprite)
{
    /* vec2_t     off; */
    texture_t* texture;  /* texture of the sprite (optional) */
    int        mvpu;     /* model view projection uniform */

    if (!sprite) {
        return;
    }

    texture = sprite->texture;

    mvpu = sprite->mvp_uniform;

    glUseProgram(sprite->program);

    glEnableVertexAttribArray(sprite->position_attrib);
    glVertexAttribPointer(sprite->position_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          sprite->vertices);

    glEnableVertexAttribArray(sprite->texture_attrib);
    glVertexAttribPointer(sprite->texture_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          sprite_texices_g);

    if (mvpu >= 0) {
        sprite_compute_mvp(sprite);
        GL_CHECK(glUniformMatrix4fv, mvpu, 1, 0, (float*) &sprite->mvp);
    }

    if (texture) {
        glActiveTexture(GL_TEXTURE0 + texture->unit);
        glBindTexture(texture->target, texture->id);
        glUniform1i(sprite->texture_uniform, texture->unit);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(sprite->texture_attrib);
    glDisableVertexAttribArray(sprite->position_attrib);
    glUseProgram(0);

    if (texture) {
        glBindTexture(texture->target, 0);
    }
}

sprite_animator_t*
sprite_animator_create(sprite_t* sprite, texture_list_t* textures,
                       vec2_t to, float speed, float elapsed)
{
     sprite_animator_t* animator;    /* Animator object returned */
     unsigned int steps = 1;         /* Number of steps */
     vec2_t from = sprite->position; /* Initial sprite position */
     float  norm = 0.0f;             /* || to - from || */

     animator = (sprite_animator_t*) malloc(sizeof(sprite_animator_t));

     if (!animator) {
         LOGE(BINA_NOT_ENOUGH_MEMORY);
         goto error;
     }

     if (elapsed <= 0) {
         LOGE(BINA_INVALID_PARAM);
         goto error;
     }

     norm  = sqrtf(powf(to.x - from.x, 2.0f) + powf(to.y - from.y, 2.0f));
     steps = (int) (norm * speed * 1.0f / elapsed);

     animator->steps    = steps;
     animator->step     = 0;
     animator->ielapsed = elapsed;
     animator->elapsed  = 0.0f;
     animator->offset.x = (to.x - from.x) / steps;
     animator->offset.y = (to.y - from.y) / steps;
     animator->textures = textures;

     LOGD("Sprite animator was created from: (%f,%f) to: (%f, %f) "
          "in %d steps at speed: %f with elapsed time: %f",
          from.x, from.y, to.x, to.y, steps, speed, elapsed);

     return animator;

error:
     if (animator) {
         free(animator);
     }

     return NULL;
}

void
sprite_animator_delete(sprite_animator_t** animator)
{
    sprite_animator_t* head = *animator;

    if (head) {
        free(head);
        *animator = NULL;
    }
}

int
sprite_animator_animate(sprite_t* sprite, sprite_animator_t* animator,
                        float elapsed)
{
    unsigned int steps = animator->steps;
    unsigned int cur   = animator->step;
    unsigned int anim  = 0; /* Current index in texturs (if any) */
    unsigned int size  = 0; /* Number of textures (if any) */

    int ret = steps - cur;

    float  ielap = animator->ielapsed;
    float  fix;
    vec2_t pos = sprite->position;
    vec2_t off = animator->offset;

    animator->elapsed += elapsed;

    if (ielap <= 0.0f) {
        return ret; /* wait a bit */
    }

    fix = elapsed / ielap;  /* Fix offset by elapsed time */

    if (cur < steps) {
        sprite->position.x = pos.x + off.x * fix;
        sprite->position.y = pos.y + off.y * fix;
        animator->step++;
    }

    if (animator->textures) {
        size = animator->textures->size;
        anim = (unsigned int) (animator->elapsed * 24.0f) % size;
        sprite->texture = animator->textures->textures[anim];

        /* We have reached the end of the animation */
        if (ret == 0) {
            sprite->texture = animator->textures->textures[0];
        }
    }

    return ret;
}
