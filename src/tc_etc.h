/**
 * Implements Ericsson Texture Compression.
 *
 * This file only contains the prototypes used in tc_etc.c.
 *
 * TODO See if there is a way to automatically generate function prototypes.
 */

/**
 * @file tc_etc.h
 * @author Jeremy Attali
 * @date August 24, 2013
 */

#pragma once

void etc_load_ktx(const unsigned char*, unsigned int, texture_t* texture);
