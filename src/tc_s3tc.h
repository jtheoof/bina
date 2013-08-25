/**
 * Implements S3 (DX) Texture Compression.
 *
 * This file is compiled only if --enable-s3tc is activated when running
 * configure script.
 *
 * This file only contains the prototypes used in s3tc.c.
 *
 * TODO See if there is a way to automatically generate function prototypes.
 */

/**
 * @file tc_s3tc.h
 * @author Jeremy Attali, Johan Attali
 * @date August 24, 2013
 */

#ifdef ENABLE_S3TC

short s3tc_check_extension();
void s3tc_dds_load(const unsigned char*, unsigned int, texture_t* texture);

#endif
