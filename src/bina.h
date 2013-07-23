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

#define GAME_WIDTH 940
#define GAME_HEIGHT 546

#define APP_TITLE "This is Binaaaaaaaaaaaaaaa"

/* types.h */

#define MAX_CHAR 64
#define MAX_PATH 256

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

#ifdef HAVE_GLU_H
#include "GL/glu.h"
#endif

#ifdef HAVE_GLUT_H
#include "GL/glut.h"
#endif

#include "error.h"
#include "utils.h"
#include "texture.h"
#include "sprite.h"
#include "shader.h"
#include "camera.h"
#include "renderer.h"

/* TODO REMOVE THIS
 * ************************ */
typedef struct bina_image
{
    char* file;
    int width;
    int height;
    int alpha;
    void* pixels;
} bina_image;

bina_image background_old;
bina_image tga;

sprite_t* background;
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
        const char* error;                                   \
        for (GLint e = glGetError(); e; e = glGetError()) {  \
            for (int i = 0; gl_errors[i].string; i++) {      \
                if (gl_errors[i].token == e) {               \
                    error = gl_errors[i].string;             \
                    break;                                   \
                }                                            \
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
