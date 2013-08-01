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
     * TODO Find out what that is used for.
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
    void* pixels;

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
 * Creates a new texture from sratch.
 *
 * It will automatically load the texture from file or memory,
 * create a new OpenGL texture object and fill the pixels.
 * @param name The name of the texture to load.
 * @return The fresh new allocated texture.
 */
texture_t* texture_create(const char* name);

/**
 * Removes the texture from memory.
 *
 * Also calls the proper OpenGL GPU destructors.
 */
void texture_delete(texture_t** texture);

/**
 * Loads a texture object from a name.
 * @param texture The texture object created.
 * @return 0 if call is successful, an error code otherwise.
 */
int texture_load(texture_t* texture);

/**
 * Loads a PNG file (based on libpng).
 */
int texture_load_png(texture_t* texture);

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
void texture_read_png_memory(png_structp pngp, png_bytep bytep,
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
