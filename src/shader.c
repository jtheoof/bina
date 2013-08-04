/**
 * @file shader.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

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
shader_create_program(const char* vertex, const char* fragment,
                      unsigned int* vid, unsigned int* fid)
{
    GLuint vertex_shader = 0;
    GLuint fragment_shader = 0;
    GLuint program = glCreateProgram();
    GLint link_status;
    GLint length = 0;

    vertex_shader = shader_create_shader(GL_VERTEX_SHADER, vertex);
    if (!vertex_shader) {
        goto error;
    }

    fragment_shader = shader_create_shader(GL_FRAGMENT_SHADER, fragment);
    if (!fragment_shader) {
        goto error;
    }

    if (program) {
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
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

    *vid = vertex_shader;
    *fid = fragment_shader;

    return program;

error:
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);

    LOGE("Could not create program");

    *vid = 0;
    *fid = 0;

    return 0;
}

void
shader_delete_program(unsigned int program)
{
    GL_CHECK(glDeleteProgram, program);
}

