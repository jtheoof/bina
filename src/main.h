/**
 * Main header that should be included in most c files.
 *
 */

/**
 * @file main.h
 * @author Jeremy Attali
 */

#pragma once

#ifdef WIN32
#include "targetver.h"
#pragma warning(disable : 4996)
#endif

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

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
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

#ifdef ANDROID
#include <jni.h>

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

AAssetManager* asset_manager_g;

#endif /* ANDROID */

#ifdef ENABLE_ETC
#include "KHR/khrplatform.h"
#endif

#include "wrapper/gl.h"
#include "wrapper/sdl.h"

/*
 * Datatypes (taken from GL/gl.h).
 */
typedef unsigned int   bina_enum;
typedef unsigned char  bina_boolean;
typedef unsigned int   bina_bitfield;
typedef void           bina_void;
typedef signed char    bina_byte;     /* 1-byte signed */
typedef short          bina_short;    /* 2-byte signed */
typedef int            bina_int;      /* 4-byte signed */
typedef unsigned char  bina_ubyte;    /* 1-byte unsigned */
typedef unsigned short bina_ushort;   /* 2-byte unsigned */
typedef unsigned int   bina_uint;     /* 4-byte unsigned */
typedef int            bina_sizei;    /* 4-byte signed */
typedef float          bina_float;    /* single precision float */
typedef float          bina_clampf;   /* single precision float in [0,1] */
typedef double         bina_double;   /* double precision float */
typedef double         bina_clampd;   /* double precision float in [0,1] */

/**
 * Window structure based on SDL_CreateWindow.
 */
typedef struct bina_window_t
{
    /**
     * Name of the window.
     */
    char* title;

    /**
     * Width of the window.
     */
    int width;

    /**
     * Height of the window.
     */
    int height;

} bina_window_t;


/**
 * Mapping between and id (or token, enum, ...) and a string.
 */
typedef struct token_string_t
{
   /**
    * Token to map (could also be an enum for example).
    */
   unsigned int token;

   /**
    * The string to map the token to.
    */
   const char* string;

} token_string_t;

/**
 * Same as #token_string_t, only adding a size to the description.
 */
typedef struct token_string_size_t
{
   /**
    * Token to map (could also be an enum for example).
    */
   unsigned int token;

   /**
    * The string to map the token to.
    */
   const char* string;

   /**
    * Size associtated with the string
    */
   unsigned int  size;

} token_string_size_t;

/**
 * Converts a token into a { TOKEN, "TOKEN" } object.
 */
#define TOKEN_TO_STRING(x) { x, #x }

/* TODO
 *  - Reorganize headers to make the build faster.
 *  - Have generic macro to check for null pointers at the beginning of a func.
 *  - Improve logging of calls to OpenGL APIs.
 *  - Reorganize the project with addition of perhaps animation.c and
 *  character.c
 */

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
            log_e("Error in %s: %s (0x%x)\n", #gl, error, e); \
        }                                                    \
    }
#else
/**
 * When we are not in debug mode, we just run the function.
 */
#define GL_CHECK(gl, ...) \
    gl(__VA_ARGS__);
#endif

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

/**
 * Initializes the engine.
 *
 * Mainly initialization of SDL.
 *
 * @param window An optional pointer to a window to create using SDL.
 *
 * @return BINA_SUCCESS if success,
 */
int bina_init(bina_window_t* window);

/* TODO Remove me */
int bina_render();

/**
 * Exits the engine and cleanup everything.
 */
void bina_exit();
