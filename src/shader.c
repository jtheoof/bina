/**
 * @file shader.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

/* TODO Maybe move these shaders to their file once file access is made easy on
 * devices. But the problem will be that those files can be accessed by
 * hackers.
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

unsigned int
shader_create_shader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
	GLint compiled;
	GLint infoLen;
	char* buf;

    if (shader) {
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader type %d (%s)\n%s\n",
                            type, buf, source);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

void
shader_delete_shader(unsigned int shader)
{
    GL_CHECK(glDeleteShader, shader);
}

unsigned int
shader_create_program(program_type_e type,
                      unsigned int* vid, unsigned int* fid)
{
    GLuint vs = 0;
    GLuint fs = 0;
    GLuint program = glCreateProgram();
    GLint link_status;
    GLint length = 0;

    switch(type) {
      case PROGRAM_BACKGROUND:
        vs = shader_create_shader(GL_VERTEX_SHADER, sprite_vertex_shader_g);
        fs = shader_create_shader(GL_FRAGMENT_SHADER, sprite_fragment_shader_g);
        break;
      default:
        LOGE("program type: %d is not implemented", type);
        goto error;
    }

    if (!vs || !fs) {
        goto error;
    }

    if (program) {
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        link_status = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &link_status);
        if (link_status != GL_TRUE) {
            length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            if (length) {
                char* buf = (char*) malloc(length);
                if (buf) {
                    glGetProgramInfoLog(program, length, NULL, buf);
                    LOGE("Could not link program: %s\n", buf);
                    free(buf);
                }
            }
            goto error;
        }
    } else {
        goto error;
    }

    LOGI("Created program: %d", program);

    *vid = vs;
    *fid = fs;

    return program;

error:
    LOGE("Could not create program: %d", type);

    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);

    *vid = 0;
    *fid = 0;

    return 0;
}

void
shader_delete_program(unsigned int program)
{
    GL_CHECK(glDeleteProgram, program);
}

