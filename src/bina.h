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

#define LOG_TAG   "gl2jni"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define LOGI(...) fprintf(stdout, __VA_ARGS__)
#define LOGE(...) fprintf(stderr, __VA_ARGS__)
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

void print_gl_string(const char *name, GLenum s);
void check_gl_error(const char* op);

void bina_set_viewport(int x, int y, int width, int height);
void bina_set_matrix_mode(int mode);
void bina_load_identity();
void bina_set_orthographic_2d(float left, float right,
                              float bottom, float top);

#include "texture.h"
#include "shader.h"
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
