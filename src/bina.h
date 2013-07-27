/**
 * Precompiled header that must be included in every c file.
 *
 * Bina a project that came true after a few too many beers.
 * A project that will change the world and ruin Pendulo Studios.
 * By Guillaume Boudeville, Jeremy Attali and Johan Attali.
 */

/**
 * @file bina.h
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#pragma once

#ifdef WIN32
#include "targetver.h"
#pragma warning(disable : 4996)
#endif

#define GAME_WIDTH 940
#define GAME_HEIGHT 546

#define APP_TITLE "This is Binaaaaaaaaaaaaaaa"

/* types.h */

#ifndef MAX_CHAR
#define MAX_CHAR 64
#endif
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef UNIX
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#endif

/* TODO make it more pretty from config.h */
#ifdef __ANDROID_API__
#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define LOG_TAG   "gl2jni"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define LOGD(...)                                                             \
    fprintf(stdout, "D/bina/%s/%s (%d): ", __FUNCTION__, __FILE__, __LINE__); \
    fprintf(stdout, __VA_ARGS__);                                             \
    fprintf(stdout, "\n");
#define LOGI(...)                                                             \
    fprintf(stdout, "I/bina/%s/%s (%d): ", __FUNCTION__, __FILE__, __LINE__); \
    fprintf(stdout, __VA_ARGS__);                                             \
    fprintf(stdout, "\n");
#define LOGE(...)                                                             \
    fprintf(stderr, "E/bina/%s/%s (%d): ", __FUNCTION__, __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                                             \
    fprintf(stderr, "\n");
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_GLEW_H
#include "GL/glew.h"
#endif

#ifdef HAVE_GLUT_H
#include "GL/glut.h"
#endif

#ifdef HAVE_GL_H
#include "GL/gl.h"
#endif

#ifdef HAVE_GLU_H
#include "GL/glu.h"
#endif

#ifdef USE_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#include "GL/glext.h"
#endif

#include "error.h"
#include "utils.h"
#include "vector.h"
#include "texture.h"
#include "sprite.h"
#include "shader.h"
#include "camera.h"
#include "renderer.h"

camera_viewport_t viewport;

sprite_t* background;
sprite_t* brian;

sprite_animator_t* brian_animator;

/* ************************ */

/* TODO Get rid of this and integrate it in the build system.
 */
#define DEBUG

#ifdef DEBUG

/**
 * Checks the status of a gl function call and an error if necessary.
 *
 * This behaviour is only needed when debugging and is especially useful for
 * devices because debugging can be hard.
 * This macro allows us to call a gl function and get the error status.
 * @example GL_CHECK
 * @code
 *  GL_CHECK(glClearColor, 1.0f, 0.0f, 0.0f, 1.0f);
 * @endcode
 */
#define GL_CHECK(gl, ...) \
    gl(__VA_ARGS__);                                         \
    {                                                        \
        const char* error = NULL;                            \
        int e, i;                                            \
        for (e = glGetError(); e; e = glGetError()) {        \
            for (i = 0; gl_errors[i].string; i++) {          \
                if (gl_errors[i].token == e) {               \
                    error = gl_errors[i].string;             \
                    break;                                   \
                }                                            \
            }                                                \
            if (error) {                                     \
                break;                                       \
            }                                                \
            LOGE("Error in %s: %s (0x%x)\n", #gl, error, e); \
        }                                                    \
    }
#else
/**
 * When we are not in debug mode, we just run the function.
 */
#define GL_CHECK(gl, ...) \
    gl(__VA_ARGS__);
#endif

/**
 * Initalization of program.
 *
 * Loads assets from disk or memory and prepares the rendering engine.
 * @param width The width of the viewport.
 * @param height The height of the viewport.
 */
void bina_init(int width, int height);

/**
 * Frees objects.
 *
 * Should be called when program exits.
 * Perhaps code or callback can vary depending on devices.
 */
void bina_end();

/**
 * Animate brian on the screen when clicking or tapping.
 *
 * This is just a testing function. The goal is to make it work on every
 * device.
 * @param to The destination where brian should go.
 * @param speed The speed we want to set brian to.
 */
void bina_animate_brian_to(vec2_t to, float speed);
/**
 * Prints useful info about about OpenGL.
 *
 * This function converts #s into a proper string.
 *
 * @param name A custom string to put before showing the value of #s.
 * @param s An OpenGL enum we want to print. For example GL_EXTENSIONS.
 */
void print_gl_string(const char* name, GLenum s);
