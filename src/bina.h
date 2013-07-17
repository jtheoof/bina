/*
 * Bina, by Johan & Jeremy Attali
 */

#pragma once

#define GAME_WIDTH 940
#define GAME_HEIGHT 546

#define APP_TITLE "This is Binaaaaaaaaaaaaaaa"

#ifndef UNIX
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#endif

/* TODO make it more pretty from config.h */
#ifdef __ANDROID_API__
#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define  LOG_TAG    "bina"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define LOGI printf
#define LOGE printf
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

void print_gl_string(const char *name, GLenum s);
void check_gl_error(const char* op);

#include "texture.h"
#include "renderer.h"

typedef struct game_background
{
    char* file;
    int width;
    int height;
    int alpha;
    GLubyte* image;
} game_background;

void bina_init(int, int);

/* vi:set ts=8 sts=4 sw=4 et: */
