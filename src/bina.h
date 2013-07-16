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
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_GLUT_H
#include "GL/glut.h"
#endif

#include "texture.h"

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
