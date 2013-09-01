/**
 * Implements S3 (DX) Texture Compression.
 *
 * This file only contains the prototypes used in tc_s3tc.c.
 *
 * TODO See if there is a way to automatically generate function prototypes.
 */

/**
 * @file tc_s3tc.h
 * @author Jeremy Attali, Johan Attali
 * @date August 24, 2013
 */

#pragma once

void s3tc_dds_load(const unsigned char*, unsigned int, texture_t* texture);
