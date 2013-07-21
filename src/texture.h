/*
 * Bina, by Johan & Jeremy Attali
 */

#pragma once

#ifdef HAVE_PNG_H
#include <png.h>
#endif

int texture_load_png(const char *filename,
                     int* width, int* height, int* alpha,
                     void** pixels);
int texture_load_tga(const char *filename,
                     int* width, int* height, void** pixels);

/* vi:set ts=8 sts=4 sw=4 et: */
