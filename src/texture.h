/**
 * Contains the necessary structs and functions to work with textures.
 *
 * It deals with generic OpenGL structures as well as custom formats.
 */

/**
 * @file texture.h
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#pragma once

#ifdef HAVE_PNG_H
#include <png.h>
#endif

#include "memory.h"

/* TODO Scale maps should have their own loading process perhaps */

/**
 * The texture structure used to control a texture behaviors and
 * properties.
 */
typedef struct texture_t
{
    /**
     * Internal name to use for the texture.
     */
    char name[MAX_CHAR];

    /**
     * Image (PNG, ...) stored in memory.
     *
     * Not sure if it is really useful to keep it into the structure. Right
     * now we are freeing the memory when the texture is successfully loaded
     * but perhaps later this object can be useful.
     */
    memory_t* image;

    /**
     * The OpenGL (ES) texture id.
     */
    unsigned int id;

    /**
     * The texture width.
     */
    unsigned int width;

    /**
     * The texture height.
     */
    unsigned int height;

    /**
     * The flag for alpha presence.
     */
    unsigned short alpha;

    /**
     * The texture byte.
     *
     * This represents the number of bytes used for each pixel.
     * For example and RGBA PNG file requires 4 bytes per pixel when a simple
     * GRAY only requires 1 byte.
     */
    unsigned char byte;

    /**
     * The total size of the texture in bytes.
     */
    unsigned int size;

    /**
     * The target of the texture.
     *
     * Can be:
     *   - GL_TEXTURE_2D
     *   - GL_TEXTURE_CUBE_MAP
     * Default: GL_TEXTURE_2D
     */
    unsigned int target;

    /**
     * Specifies which texture unit to make active.
     *
     * The number of texture units is implementation dependent, but must be at
     * least 8. texture must be one of GL_TEXTUREi, where i ranges from 0 to
     * (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1). The initial value is
     * GL_TEXTURE0.
     */
    unsigned int unit;

    /**
     * The internal format of the texture.
     *
     * Can be:
     *   - GL_RGB
     *   - GL_RGBA
     *   - GL_LUMINANCE
     *   - GL_ALPHA
     *   - GL_LUMINANCE_ALPHA
     */
    unsigned int iformat;

    /**
     * The format of the texture.
     *
     * Can be:
     *   - GL_RGB
     *   - GL_RGBA
     *   - GL_LUMINANCE
     *   - GL_ALPHA
     *   - GL_LUMINANCE_ALPHA
     */
    unsigned int format;

    /**
     * The type of texel to use for this texture.
     *
     * Can be:
     *   - GL_UNSIGNED_BYTE
     *   - GL_UNSIGNED_SHORT_5_6_5
     *   - GL_UNSIGNED_SHORT_4_4_4_4
     *   - GL_UNSIGNED_SHORT_5_5_5_1
     * <br>
     * Default: GL_UNSIGNED_BYTE
     */
    unsigned int type;

    /**
     * The raw texel array.
     *
     * This is what #format member describes.
     * It is used by OpenGL to properly read the array of data.
     * For example in TGA format it will be pure raw RGB data.
     * TODO Perphaps it's possible to free the memory after the data has
     * been put to GPU through GlTexImage2d. This needs to be checked, it
     * would free some memory.
     */
    unsigned char* pixels;

    /**
     * The number of mipmaps (PVRTC only).
     *
     * <em>Not sure if it will be used at all.</em>
     */
    unsigned int nmipmap;

    /**
     * The compression type.
     *
     * <em>Not sure if it will be used at all.</em>
     */
    unsigned int compression;

} texture_t;

/**
 * A list of textures.
 *
 * Particularly useful to load sprite animations.
 * Should probably make it a circular linked list.
 */
typedef struct texture_list_t
{
    /**
     * The number of textures.
     */
    unsigned int size;

    /**
     * Current texture id in #textures.
     */
    unsigned int tid;

    /**
     * The array of textures that will be loaded for the animation.
     */
    texture_t** textures;

} texture_list_t;

/**
 * Creates a new texture from sratch.
 *
 * It will automatically load the texture from file or memory,
 * create a new OpenGL texture object and fill the pixels.
 * @param name The name of the texture to load.
 * @param keep A flag to indicate wether or not we want to retain the texture
 * loaded into memory or just send it to the GPU and release the memory.
 * For example in the case of scale maps, we want to keep the object in memory.
 * @return The fresh new allocated texture.
 */
texture_t* texture_create(const char* name, const short keep);

/**
 * Removes the texture from memory.
 *
 * Also calls the proper OpenGL GPU destructors.
 */
void texture_delete(texture_t** texture);

/**
 * Creates multiple textures based on single filenaming.
 *
 * This should be used to load animations (mutliple textures at once).
 * Note that right now, only PNG files are loaded this way.
 * We could write a wrapper to the assets manager that will automatically list
 * all the perso1_walkCycle_cameraLeft files listed and their extension.
 * This way we would not have to pass #ext nor #size.
 *
 * @param folder The root folder in 'animations' where the animation is to be
 * loaded.
 * @param animation The name of the group of textures to load.
 * For example: perso1_walkCycle_cameraLeft
 * This will load perso1_walkCycle_cameraLeft_001.png, ...
 * @param ext The extension of the files to load.
 * @param size The number of files to load.
 * @return The list of texture_t* textures created.
 */
texture_list_t* texture_create_list(const char* folder,
                                    const char* filename,
                                    const char* ext,
                                    const unsigned short size);

/**
 * Frees memory occupied by a texture_list_t object.
 *
 * @param list The list to free.
 */
void texture_delete_list(texture_list_t** list);

/**
 * Loads a texture object from a name.
 * 
 * The object is first loaded into memory and then passed to the approriate
 * extension handler.
 *
 * @param name The name of the asset to load.
 * @param keep A flag to either keep the texture in memory or not.
 * @param texture The texture object created.
 * @return 0 if call is successful, an error code otherwise.
 */
int texture_load(const char* name, const short keep, texture_t* texture);

/**
 * Loads a PNG file (based on libpng).
 *
 * @param memory The original object loaded from assets in its original
 * format.
 * @param texture The texture object that will contain the RAW data used by
 * OpenGL.
 * @return 0 if call is successful, an error code otherwise.
 */
int texture_png_load(memory_t* memory, texture_t* texture);

/**
 * Loads a DDS file (S3TC must be enabled).
 *
 * @param memory The original object loaded from assets in its original
 * format.
 * @param texture The texture object that will contain the RAW data used by
 * OpenGL.
 */
void texture_dds_load(memory_t* memory, texture_t* texture);

/**
 * Reads a PNG file from memory.
 *
 * This is the callback from png_set_read_fn and used to read the PNG image.
 *
 * @param pngp The read structure.
 * @param bytep The row pointers that will be the destination of the data
 * read.
 * @param size The number of bytes to read.
 */
void texture_png_read_memory(png_structp pngp, png_bytep bytep,
                             png_size_t size);

/**
 * Loads a TGA raw file (no need for any library).
 */
int texture_load_tga(const char *filename,
                     int* width, int* height, void** pixels);

/**
 * OpenGL helper function to create a new texture buffer and apply it to the
 * pixels raw data.
 */
int texture_gl_create(texture_t* texture);

/**
 * OpenGL helper function to free the memory used by the texture.
 */
int texture_gl_delete(texture_t* texture);

/**
 * Debugging function to print info about a texture.
 */
void texture_print_info(texture_t* texture);
