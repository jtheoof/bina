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

#include "memory.h"

/**
 * Enumeration of flags used for textures.
 */
typedef enum texture_flag_enum_t
{
    /**
     * Do not free pixels after load.
     */
    TEXTURE_KEEP_PIXELS    = 1 << 0,

    /**
     * Should be flipped vertically.
     */
    TEXTURE_FLIP_VERTICAL  = 1 << 1,

    /**
     * Already uploaded to server.
     */
    TEXTURE_UPLOADED       = 1 << 2,

    /**
     * Repeat texture wrapping.
     */
    TEXTURE_WRAP_REPEAT    = 1 << 3,

    /**
     * Use mipmap.
     */
    TEXTURE_MIPMAP         = 1 << 4

} texture_flag_enum_t;

typedef enum texture_filter_enum_t
{
    /**
     * Image filtering nearest.
     */
    TEXTURE_FILTER_0X = 0,

    /**
     * Image filtering linear.
     */
    TEXTURE_FILTER_1X = 1,

    /**
     * Bilinear image filtering.
     */
    TEXTURE_FILTER_2X = 2,

    /**
     * Trilinear image filtering.
     */
    TEXTURE_FILTER_3X = 3

} texture_filter_enum_t;

typedef struct texture_ogl_t {

    /**
     * The OpenGL (ES) texture id.
     */
    unsigned int tid;

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
     *
     * Or one of the multiple compressed data value, for example:
     *  - GL_COMPRESSED_RGB_S3TC_DXT1_EXT
     *  - GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
     *  - GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
     *
     * The texture loader is responsible for setting this internal format
     * correcly. For example see how it's done in S3TC texture compression
     * loader.
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

} texture_ogl_t;

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
     * The raw texel array.
     *
     * This is what #format member describes.
     * It is used by OpenGL to properly read the array of data.
     * For example in TGA format it will be pure raw RGB data.
     */
    unsigned char* pixels;

    /**
     * The number of mipmaps (if any).
     *
     * TODO Get rid of this. Texture should be loaded to hardware for each
     * texture compression type. No need to know how many mipmaps there is
     * according to libktx.
     */
    unsigned int nmipmap;

    /**
     * Boolean to know if texture has mipmaps.
     */
    unsigned char mipmapped;

    /**
     * The compression type.
     */
    unsigned int compression;

    /**
     * The compression block size if compression is used.
     */
    unsigned int blksize;

    /**
     * Some additional flags interesting to know about the texture.
     *
     * For example a DDS texture file has Y coordinates flipped compared to
     * OpenGL. So it's usefuly to know that the image should be flipped before
     * being sent to OpenGL before drawing.
     */
    unsigned long flags;

    /**
     * OpenGL specifics.
     */
    texture_ogl_t ogl;

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
 * @param flags Flags relative to the texture. For example it can be useful to
 * indicate wether or not we want to retain the texture loaded into memory or
 * just send it to the GPU and release the memory. In the case of scale maps,
 * we want to keep the object in memory. See #texture_flag_enum_t
 * @param filter The filtering to apply to the texture.
 * See #texture_filter_enum_t
 * @return The fresh new allocated texture.
 */
texture_t* texture_create(const char* name, bina_enum flags, bina_enum filter);

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
 * @param texture The texture object created.
 * @return 0 if call is successful, an error code otherwise.
 */
int texture_load(const char* name, texture_t* texture);

/**
 * Loads a PNG file (based on libpng).
 *
 * PNG must be enabled.
 *
 * @param memory The original object loaded from assets in its original
 * format.
 * @param texture The texture object that will contain the RAW data used by
 * OpenGL.
 * @return 0 if call is successful, an error code otherwise.
 */
void texture_load_png(memory_t* memory, texture_t* texture);

/**
 * Loads a DDS file (S3TC must be enabled).
 *
 * @param memory The original object loaded from assets in its original
 * format.
 * @param texture The texture object that will contain the RAW data used by
 * OpenGL.
 */
void texture_load_dds(memory_t* memory, texture_t* texture);

/**
 * Loads a KTX file (ETC must be enabled).
 *
 * KTX is a format for storing textures for OpenGL and OpenGL ES applications.
 * It is distinguished by the simplicity of the loader required to instantiate
 * a GL texture object from the file contents.
 *
 * @param memory The original object loaded from assets in its original
 * format.
 * @param texture The texture object that will contain the RAW data used by
 * OpenGL.
 */
void texture_load_ktx(memory_t* memory, texture_t* texture);

/**
 * Debugging function to print info about a texture.
 */
void texture_print_info(texture_t* texture);
