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

#define GAME_WIDTH        960
#define GAME_HEIGHT       540
#define GAME_REFRESH_RATE 16  /* 60 times per second */

#define APP_TITLE "This is Binaaaaaaaaaaaaaaa"

/* types.h */

#ifndef MAX_CHAR
#define MAX_CHAR 64
#endif
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_MATH_H
#include <math.h>
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

/* TODO Clean this, it's really ugly. */
#if _MSC_VER
#define snprintf _snprintf
#endif

#ifdef ANDROID
#include <jni.h>

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define LOG_TAG   "gl2jni"

/* TODO make it more pretty from config.h */
# ifdef DEBUG
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
# else
#define LOGD(...) { }
#define LOGI(...) { }
# endif
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

AAssetManager* asset_manager_g;

#else

# ifdef DEBUG
#define LOGD(...)                                                             \
    fprintf(stdout, "D/bina/%s/%s (%d): ", __FUNCTION__, __FILE__, __LINE__); \
    fprintf(stdout, __VA_ARGS__);                                             \
    fprintf(stdout, "\n");
#define LOGI(...)                                                             \
    fprintf(stdout, "I/bina/%s/%s (%d): ", __FUNCTION__, __FILE__, __LINE__); \
    fprintf(stdout, __VA_ARGS__);                                             \
    fprintf(stdout, "\n");
# else
#define LOGD(...) { }
#define LOGI(...) { }
# endif
#define LOGE(...)                                                             \
    fprintf(stderr, "E/bina/%s/%s (%d): ", __FUNCTION__, __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                                             \
    fprintf(stderr, "\n");

#endif /* ANDROID */

#ifdef HAVE_GL_GLEW_H
#include "GL/glew.h"
#endif

#ifdef HAVE_GL_GLUT_H
#include "GL/glut.h"
#endif

#ifdef HAVE_GLUT_GLUT_H
#include "GLUT/glut.h"
#endif

#ifdef HAVE_GL_GL_H
#include "GL/gl.h"
#endif

#ifdef HAVE_PNG_H
#include <png.h>
#endif

/* Utils */
#include "error.h"
#include "utils.h"

/* Math */
#include "vector.h"
#include "camera.h"

/* Memory */
#include "memory.h"
#include "texture.h"
#include "shader.h"

/* Objects */
#include "sprite.h"
#include "scene.h"

/* Engine */
#include "renderer.h"

camera_viewport_t viewport;

sprite_t* back;
sprite_t* porc;

texture_t* back_tex;
texture_list_t* porc_r_tex;
texture_list_t* porc_l_tex;

#define SPRITES_DEMO 0 /* Slow at 998 */

sprite_animator_t* ani_porc;

scene_t* scene;

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
 * Loads the background into the game.
 *
 * The background in pretty important because it defines the size of the
 * biggest element that will be drawn to the viewport.
 * This will set the default size for smaller sprites.
 *
 * @param filepath The file path of the background to load.
 */
void bina_load_background(const char* filepath);

/**
 * Loads Porc Gerard into the scene.
 */
void bina_load_porc();

/**
 * Animate porc on the screen when clicking or tapping.
 *
 * This is just a testing function. The goal is to make it work on every
 * device.
 * @param to The destination where porc should go.
 * @param speed The speed we want to set porc to.
 */
void bina_animate_porc_to(vec2_t to, float speed);

/* TODO Move this to its own main.h ? */

/**
 * Retrieves the time elapsed in seconds since previous call.
 *
 * Note that the target FPS is at least 60 FPS (0.016s between frames).
 * This code needs is platform dependant for now. It means that it has its own
 * implementation in main_glut.c and main_android.c. The reason is because
 * getting time is usually specific to the platform. We can be more precise
 * and separate it into its own time_os.c later on.
 *
 * @return The time elapsed since last call to the same function in seconds.
 */
float main_get_time_elapsed();

/* TODO Move this to approriate file */

/**
 * Prints useful info about about OpenGL.
 *
 * This function converts #s into a proper string.
 *
 * @param name A custom string to put before showing the value of #s.
 * @param s An OpenGL enum we want to print. For example GL_EXTENSIONS.
 */
void print_gl_string(const char* name, GLenum s);

/**
 * Temporary animation function to move random sprites.
 */
void bina_animate_demo_sprite(sprite_t* sprite, sprite_animator_t** animator,
                              float elapsed);
