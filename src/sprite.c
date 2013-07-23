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
    "attribute vec4 position_a;\n"
    "attribute vec2 texture_a;\n"
    "varying   vec2 texture_v;\n"
    "void main() {\n"
    "    gl_Position = position_a;\n"
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

static const float sprite_vertices_g[] = {
    -1.0f,  1.0f,
    -1.0f, -1.0f,
     1.0f,  1.0f,
     1.0f, -1.0f
};

static const float sprite_texices_g[] = {
     0.0f,  1.0f,
     0.0f,  0.0f,
     1.0f,  1.0f,
     1.0f,  0.0f
};

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
 */
sprite_t*
sprite_create(const char* texture_name)
{
    int err;
    sprite_t* sprite;
    unsigned int program;
    
    sprite = (sprite_t*) malloc(sizeof(sprite_t));

    if (!sprite) {
        LOGE("Not enough memory to create sprite: %s", texture_name);
        return NULL;
    }

    sprite->texture = texture_create(texture_name);
    sprite->program = shader_create_program(sprite_vertex_shader_g,
                                            sprite_fragment_shader_g);

    program = sprite->program;
    if (program) {
        sprite->position_attrib = glGetAttribLocation(program, "position_a");
        sprite->texture_attrib = glGetAttribLocation(program, "texture_a");
        sprite->texture_uniform = glGetUniformLocation(program, "texture_u");
    }

    return sprite;
}

void
sprite_delete(sprite_t** sprite)
{
    sprite_t* s = *sprite;
    if (s) {
        texture_delete(&s->texture);
        free(s);
        s = NULL;
    }
    *sprite = s;
}

void
sprite_render(sprite_t* sprite)
{
    glUseProgram(sprite->program);

    glEnableVertexAttribArray(sprite->position_attrib);
    glVertexAttribPointer(sprite->position_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          sprite_vertices_g);

    glEnableVertexAttribArray(sprite->texture_attrib);
    glVertexAttribPointer(sprite->texture_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          sprite_texices_g);

    if (sprite->texture) {
        texture_t* texture = sprite->texture;
        glActiveTexture(texture->unit);
	    glBindTexture(texture->target, texture->id);
        glUniform1i(sprite->texture_uniform, 0);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
