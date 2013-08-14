/**
 * @file sprite.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

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

sprite_t*
sprite_create(texture_t* texture, const unsigned int program,
              const vec2_t position, const vec2_t offset, const vec2_t size,
              const float scale)
{
    sprite_t* sprite;
    float width = size.x;
    float height = size.y;

    sprite = (sprite_t*) malloc(sizeof(sprite_t));

    sprite->program = program;

    if (program) {
        sprite->mvp_uniform      = glGetUniformLocation(program, "mvp_u");
        sprite->position_uniform = glGetUniformLocation(program, "position_u");
        sprite->scaling_uniform  = glGetUniformLocation(program, "scaling_u");
        sprite->position_attrib  = glGetAttribLocation(program, "position_a");
        sprite->texture_attrib   = glGetAttribLocation(program, "texture_a");
        sprite->texture_uniform  = glGetUniformLocation(program, "texture_u");

        LOGD("In program: %d, "
             "[mvp]: uniform: %d, "
             "[position]: uniform: %d, attribute: %d "
             "[texture]: uniform: %d, attribute: %d "
             "[scaling]: uniform: %d",
             program,
             sprite->mvp_uniform,
             sprite->position_uniform, sprite->position_attrib,
             sprite->texture_uniform,  sprite->texture_attrib,
             sprite->scaling_uniform);
    }

    /* sprite->vertices[0][0] = -1.0f - offset.x; */
    /* sprite->vertices[0][1] = -1.0f + 2.0f * height - offset.y; */
    /* sprite->vertices[1][0] = -1.0f - offset.x; */
    /* sprite->vertices[1][1] = -1.0f - offset.y; */
    /* sprite->vertices[2][0] = -1.0f + 2.0f * width - offset.x; */
    /* sprite->vertices[2][1] = -1.0f + 2.0f * height - offset.y; */
    /* sprite->vertices[3][0] = -1.0f + 2.0f * width - offset.x; */
    /* sprite->vertices[3][1] = -1.0f - offset.y; */

    /* sprite->vertices[0][0] = -1.0f; */
    /* sprite->vertices[0][1] = -1.0f + 2.0f * height; */
    /* sprite->vertices[1][0] = -1.0f; */
    /* sprite->vertices[1][1] = -1.0f; */
    /* sprite->vertices[2][0] = -1.0f + 2.0f * width; */
    /* sprite->vertices[2][1] = -1.0f + 2.0f * height; */
    /* sprite->vertices[3][0] = -1.0f + 2.0f * width; */
    /* sprite->vertices[3][1] = -1.0f; */

    float tmp = 1.0f;
    sprite->vertices[0][0] = (-1.0f) * tmp;
    sprite->vertices[0][1] = (-1.0f + 2.0f * height) * tmp;
    sprite->vertices[1][0] = (-1.0f) * tmp;
    sprite->vertices[1][1] = (-1.0f) * tmp;
    sprite->vertices[2][0] = (-1.0f + 2.0f * width) * tmp;
    sprite->vertices[2][1] = (-1.0f + 2.0f * height) * tmp;
    sprite->vertices[3][0] = (-1.0f + 2.0f * width) * tmp;
    sprite->vertices[3][1] = (-1.0f) * tmp;

    sprite->position = position;
    sprite->offset   = offset;
    sprite->scale    = scale;
    sprite->width    = width;
    sprite->height   = height;
    sprite->texture  = texture;
    sprite->mvp      = mat4_identity();

    return sprite;
}

void
sprite_delete(sprite_t** sprite)
{
    sprite_t* s = *sprite;
    /* XXX The sprite should not be responsible for deleting the texture.
     * The texture should be deleted by the object that created it.
     */
    if (s) {
        shader_delete_program(s->program);
        free(s);
        s = NULL;
    }
    *sprite = s;
}

void
sprite_set_texture(sprite_t* sprite, texture_t* texture)
{
    if (sprite && texture) {
        sprite->texture = texture;
    }
}

void
sprite_render(sprite_t* sprite)
{
    vec2_t     pos;
    /* vec2_t     off; */
    texture_t* texture;
    int        posu, scau, mvpu;

    if (!sprite) {
        return;
    }

    /* TODO This should really be calculated from a change of basis matrix */
    /* off.x =  2.0f * sprite->offset.x; */
    /* off.y = -2.0f * sprite->offset.y; */

    /* pos = vec2_sub(sprite->position, off); */
    pos = sprite->position;

    texture = sprite->texture;

    posu = sprite->position_uniform;
    scau = sprite->scaling_uniform;
    mvpu = sprite->mvp_uniform;

    glUseProgram(sprite->program);

    glEnableVertexAttribArray(sprite->position_attrib);
    glVertexAttribPointer(sprite->position_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          sprite->vertices);

    glEnableVertexAttribArray(sprite->texture_attrib);
    glVertexAttribPointer(sprite->texture_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          sprite_texices_g);

    if (posu >= 0) {
        GL_CHECK(glUniform4f, posu, pos.x, pos.y, 0.0f, 1.0f);
    }
    if (scau >= 0) {
        GL_CHECK(glUniform1f, scau, sprite->scale);
    }
    if (mvpu >= 0) {
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
