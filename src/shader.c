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
    if (shader) {
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
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

unsigned int
shader_create_program(const char* vertex, const char* fragment)
{
    GLuint vertex_shader, fragment_shader;
    GLuint program = glCreateProgram();

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
        GLint link_status = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &link_status);
        if (link_status != GL_TRUE) {
            GLint length = 0;
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
    return program;

error:
    if (program) {
        glDeleteProgram(program);
    }

    LOGE("Could not create program");
    return 0;
}

