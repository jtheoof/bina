/**
 * @file sprite.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

struct sprite_module_info {
    unsigned long sprite_animator_id;
};

/**
 * Module scope variable.
 */
static struct sprite_module_info m = {
    0
};

/* XXX This should actually be comming from some sort of scene/manifest
 * loader.
 * TODO remove me when proper scene management has been developped.
 */
static const token_string_t g_cam_type_names[] = {
    { SPRITE_CAM_TYPE_FRONT, "frontCam" },
};

static const token_string_size_t g_anim_names_sizes[] = {
    { SPRITE_ANIM_NEUTRALPOSE, "neutralPose", 30 },
    { SPRITE_ANIM_STOPACTION1, "stopAction1", 30 },
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

static sprite_tex_anim_t*
create_character_anim(const char* character)
{
    sprite_tex_anim_t* ret  = NULL;
    texture_list_t**   list = NULL;
    int i, c, a, size;
    char filename[MAX_PATH];

    int cams  = SPRITE_CAM_TYPE_COUNT;
    int anims = SPRITE_ANIM_COUNT;

    ret = (sprite_tex_anim_t*) malloc(sizeof(sprite_tex_anim_t));

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
        for (a = 0; a < anims; a++) {
            i = c * cams + a;
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

static texture_list_t*
get_character_anim(sprite_t* sprite)
{
    sprite_tex_anim_t* a;

    if (!sprite || !sprite->tex_anims) {
        return NULL;
    }

    a = sprite->tex_anims;

    /* Return the proper id in the list, see the construction for more info */
    return a->list[a->cur_cam_type * a->nb_cam_types + a->cur_anim];
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
    sprite->tex_anims  = NULL;
    sprite->animator   = NULL;

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

    sprite->tex_anims = create_character_anim(name);

    if (sprite->tex_anims) {
        sprite->tex_anims->cur_cam_type = SPRITE_CAM_TYPE_FRONT;
        sprite->tex_anims->cur_anim = SPRITE_ANIM_STOPACTION1;

        if (sprite->tex_anims->list) {
            sprite->texture = sprite->tex_anims->list[0]->textures[0];
        }
    }

    return sprite;
}

void
sprite_delete(sprite_t** sprite)
{
    sprite_t* s = *sprite;
    sprite_tex_anim_t* a;

    int i ;

    /* XXX The sprite should not be responsible for deleting the texture.
     * The texture should be deleted by the object that created it.
     */
    if (s) {
        shader_delete_program(s->program);

        if (s->tex_anims) {
            a = s->tex_anims;

            if (a->list) {
                for (i = 0; i < a->size; i++) {
                    /* Not sure I need the () around a->list[i] */
                    texture_delete_list(&(a->list[i]));
                }
                free(a->list);
                a->list = NULL;
            }
            free(a);
            s->tex_anims = NULL;
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
    if (sprite && sprite->tex_anims) {
        sprite->tex_anims->cur_cam_type = cam;
    }
}

void sprite_set_animation(sprite_t* sprite, sprite_anim_e anim)
{
    if (sprite && sprite->tex_anims) {
        sprite->tex_anims->cur_anim = anim;
    }
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
sprite_animate_char_to(sprite_t* sprite, vec2_t to, float speed, float elapsed)
{
    vec2_t pos;

    lin_vec2_anim_t*   linanim;
    texture_list_t*    texanims;

    if (!sprite) {
        return;
    }

    /* If there is an animation currently in place, delete it first */
    if (sprite->animator) {
        sprite_animator_delete(&sprite->animator);
    }

    linanim = (lin_vec2_anim_t*) malloc(sizeof(lin_vec2_anim_t));

    if (!linanim) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
    }

    pos = sprite->position;
    *linanim = lin_vec2_anim_create(pos, to, speed, elapsed);
    texanims = get_character_anim(sprite);
    sprite_animator_create(sprite, linanim, texanims);
}

void
sprite_animate_idle(sprite_t* sprite)
{
    if (!sprite || !sprite->tex_anims) {
        return;
    }

    sprite_set_animation(sprite, rand() % SPRITE_ANIM_COUNT);

    texture_list_t*    texanims = get_character_anim(sprite);
    lin_vec2_anim_t*   linanim = NULL;

    sprite_animator_create(sprite, linanim, texanims);
}

sprite_anim_status_e
sprite_animate(sprite_t* sprite, float elapsed)
{
    sprite_animator_t* animator;
    texture_list_t*    texanims;
    lin_vec2_anim_t*   linanim;

    unsigned int steps; /* number of steps in the animation. */
    unsigned int cur;   /* current step in the animation */
    unsigned int rem;   /* steps remaining in animation */
    unsigned int tid;   /* current index in textures (if any) */
    unsigned int size;  /* number of textures (if any) */
    unsigned int ret;   /* flag to return at the end */

    vec2_t pos;         /* sprite current position */
    vec2_t off;         /* sprite offset in linear animation */
    float  ielap;
    float  fix;

    if (!sprite || !sprite->animator) {
        return SPRITE_ANIM_STATUS_INVALID;
    }

    animator = sprite->animator;
    linanim  = animator->animation;
    texanims = animator->textures;

    /* Increase elapsed time since start of animation. */
    animator->elapsed += elapsed;

    ret = SPRITE_ANIM_STATUS_PLAYING;

    if (linanim) {
        steps = linanim->steps;
        cur   = linanim->step;
        rem   = steps - cur;
        ielap = animator->ielapsed;
        pos   = sprite->position;
        off   = linanim->offset;

        if (ielap <= 0.0f) {
            return ret;
        }

        fix = elapsed / ielap;  /* fix offset by elapsed time */

        if (cur < steps) {
            sprite->position.x = pos.x + off.x * fix;
            sprite->position.y = pos.y + off.y * fix;
            linanim->step++;
        } else {
            ret |= SPRITE_ANIM_STATUS_LINEAR_FINISHED;
        }
    }

    if (texanims) {
        size = texanims->size;
        tid = (unsigned int) (animator->elapsed * 24.0f);

        if (tid >= size) {
            ret |= SPRITE_ANIM_STATUS_TEXTURE_FINISHED;
            tid = tid % size;
        }

        sprite->texture = texanims->textures[tid];
        texanims->tid = tid;
    }

    if (linanim && texanims) {
        if (rem == 0) { /* no more remaining steps, anim is done */
            sprite->texture = texanims->textures[0]; /* back to first tex */
            ret = SPRITE_ANIM_STATUS_DONE;
        }
    }

    if (!linanim && texanims) {
        if (ret & SPRITE_ANIM_STATUS_TEXTURE_FINISHED) {
            ret = SPRITE_ANIM_STATUS_DONE;
        }
    }

    return ret;
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
sprite_animator_create(sprite_t* sprite, lin_vec2_anim_t* animation,
                       texture_list_t* textures)
{
     sprite_animator_t* animator;    /* Animator object returned */
     animator = (sprite_animator_t*) malloc(sizeof(sprite_animator_t));

     if (!animator) {
         LOGE(BINA_NOT_ENOUGH_MEMORY);
         goto error;
     }

     animator->id        = m.sprite_animator_id++;
     animator->animation = animation;
     animator->textures  = textures;

     /* LOGD("Sprite animator was created from: (%f,%f) to: (%f, %f) " */
     /*      "in %d steps at speed: %f with elapsed time: %f", */
     /*      from.x, from.y, to.x, to.y, steps, speed, elapsed); */

     /* Directly assign the animator to the sprite */
     sprite->animator = animator;

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
        if (head->animation) {
            /* DO NOT free head->textures */
            free(head->animation);
            head->animation = NULL;
        }
        free(head);
        *animator = NULL;
    }
}
