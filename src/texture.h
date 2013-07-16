/*
 * Bina, by Johan & Jeremy Attali
 */

#pragma once

#ifdef HAVE_PNG_H
#include <png.h>
#endif

int texture_load_png(char *name, int outWidth, int outHeight,
                     int outHasAlpha, GLubyte **outData);

/* vi:set ts=8 sts=4 sw=4 et: */
