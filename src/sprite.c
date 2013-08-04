/**
 * @file sprite.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

/* TODO Move these shaders to their file once file access is made easy on
 * devices.
 */

/**
 * Vertex shader used for simple sprites.
 *
 * This is a basic vertex shader used for position and texturing.
 *
 * The first three lines are used to deal with precision on devices. Since
 * it's not GLSL spec varies from OpenGL to OpenGL ES we need a specific
 * define when rendering for GL_ES.
 *
 * <code>
 *  attribute vec4 position_a;
 * </code>
 * This declares our attribute to set the vertices positions.
 * <br>
 * <code>
 *  attribute vec2 texture_a;
 * </code>
 * This declares our attribute to set the texices positions.
 * <br>
 * <code>
 *  attribute vec2 texture_v;
 * </code>
 * This declares our varing to set the texices positions. This is passed to
 * the fragment shader later in the graphics pipeline and is used to actually
 * apply those texture coordinates to the pixels associated with the texture.
 * <br>
 * <code>
 *  attribute vec4 position_a;
 *  void main() {
 *      gl_Position = position_a;
 *      texture_v = texture_a;
 *  };
 * </code>
 * This is the main function of the shader.
 * gl_Position is a specific variable used by OpenGL to set the position of
 * the vertices. We tell it to be the attribute we are passing.
 * The varying texture is set to the texture attribute. It will then be passed
 * to the fragment shader.
 */
static const char sprite_vertex_shader_g[] =
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform vec4 position_u;\n"
    "attribute vec4 position_a;\n"
    "attribute vec2 texture_a;\n"
    "varying   vec2 texture_v;\n"
    "void main() {\n"
    "    vec4 offset = vec4(position_u.x, position_u.y, 0, 0);\n"
    "    gl_Position = position_a + offset;\n"
    "    texture_v = texture_a;\n"
    "}\n";

/**
 * Fragment (or pixel) shader used for simple sprites.
 *
 * This is a basic fragment shader used for only texturing.
 *
 * The first three lines are used to deal with precision on devices. Since
 * it's not GLSL spec varies from OpenGL to OpenGL ES we need a specific
 * define when rendering for GL_ES.
 *
 * <code>
 *  uniform sampler2D texture_u;
 * </code>
 * This declares our uniform to set the data of the texture itself. Since the
 * data is stored on machine memory, we need a way to transfer this C texture
 * into the GPU. This is done through the use of uniforms. This allows us to
 * set the texture to the GPU to be processed by the fragment shader.
 * <code>
 *  varying vec2 texture_v;
 * </code>
 * This is the varying texture we set previously in our vertex shader. It is
 * now arriving in our fragment shader and we use it to make up the texture
 * with the right texices.
 * <code>
 *  void main() {
 *      gl_FragColor = texture2D(texture_u, texture_v);
 *  };
 * </code>
 * This is the main function of the vertex shader. texture2D is used to apply
 * a texture data to the right texture coordinates (texices).
 */
static const char sprite_fragment_shader_g[] =
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D texture_u;\n"
    "varying vec2      texture_v;\n"
    "void main() {\n"
    "    gl_FragColor = texture2D(texture_u, texture_v);\n"
    "}\n";

/**
 * Constant holding default values for vertices.
 *
 * Not sure it will be used at all.
 */
static const float sprite_vertices_g[] = {
    -1.0f,  1.0f,
    -1.0f, -1.0f,
     1.0f,  1.0f,
     1.0f, -1.0f
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

sprite_t*
sprite_create(const texture_t* texture, const vec2_t position,
              const vec2_t offset, const vec2_t size)
{
    sprite_t* sprite;
    unsigned int program;
    float width = size.x;
    float height = size.y;

    sprite = (sprite_t*) malloc(sizeof(sprite_t));

    sprite->program = shader_create_program(sprite_vertex_shader_g,
                                            sprite_fragment_shader_g,
                                            &sprite->vshader, &sprite->fshader);

    program = sprite->program;
    if (program) {
        sprite->position_uniform = glGetUniformLocation(program, "position_u");
        sprite->position_attrib = glGetAttribLocation(program, "position_a");
        sprite->texture_attrib = glGetAttribLocation(program, "texture_a");
        sprite->texture_uniform = glGetUniformLocation(program, "texture_u");

        LOGD("Sprite has position: uniform: %d, attribute: %d "
             "texture: uniform: %d, attribute: %d",
             sprite->position_uniform, sprite->position_attrib,
             sprite->texture_uniform,  sprite->texture_attrib);
    }

    sprite->vertices[0][0] = -1.0f - offset.x;
    sprite->vertices[0][1] = -1.0f + height - offset.y;
    sprite->vertices[1][0] = -1.0f - offset.x;
    sprite->vertices[1][1] = -1.0f - offset.y;
    sprite->vertices[2][0] = -1.0f + width - offset.x;
    sprite->vertices[2][1] = -1.0f + height - offset.y;
    sprite->vertices[3][0] = -1.0f + width - offset.x;
    sprite->vertices[3][1] = -1.0f - offset.y;

    sprite->position = position;
    sprite->width = width;
    sprite->height = height;
    sprite->texture = texture;

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
        shader_delete_shader(s->vshader);
        shader_delete_shader(s->fshader);
        shader_delete_program(s->program);
        /* if (s->texture) { */
        /*     texture_delete(&s->texture); */
        /* } */
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
    vec2_t pos = sprite->position;
    texture_t* texture = sprite->texture;

    glUseProgram(sprite->program);

    glEnableVertexAttribArray(sprite->position_attrib);
    glVertexAttribPointer(sprite->position_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          sprite->vertices);

    glEnableVertexAttribArray(sprite->texture_attrib);
    glVertexAttribPointer(sprite->texture_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          sprite_texices_g);

    GL_CHECK(glUniform4f, sprite->position_uniform, pos.x, pos.y, 0.0f, 1.0f);

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

unsigned int
sprite_animator_animate(sprite_t* sprite, sprite_animator_t* animator,
                        float elapsed)
{
    unsigned int steps = animator->steps;
    unsigned int cur   = animator->step;
    unsigned int anim  = 0; /* Current index in texturs (if any) */
    unsigned int size  = 0; /* Number of textures (if any) */

    float  ielap = animator->ielapsed;
    float  fix;
    vec2_t pos = sprite->position;
    vec2_t off = animator->offset;

    animator->elapsed += elapsed;

    if (ielap <= 0.0f) {
        return steps - cur; /* wait a bit */
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
    }

    return steps - cur;
}
