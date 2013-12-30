/**
 * Implements PNG loading.
 *
 * This file is compiled only if --with-png is activated when running
 * configure script.
 */

/**
 * @file tc_png.c
 * @author Jeremy Attali, Johan Attali
 * @date September 1, 2013
 */

#include "bina.h"

#ifdef ENABLE_PNG

#include "log.h"
#include "memory.h"
#include "texture.h"

#include <png.h>

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
static void
read_memory(png_structp pngp, png_bytep bytep, png_size_t size)
{
    memory_t* m = (memory_t*) png_get_io_ptr(pngp);

    memory_read(m, bytep, size);
}

/**
 * Creates a PNG asset from memory into a texture object.
 *
 * It is important to notice that the texture loaded will be RAW. There is not
 * texture compression when loading PNG to the GPU. So a FullHD image takes
 * around 8MB in memory.
 *
 * @param memory The asset previously loaded in memory (PNG image).
 * @param texture The texture returned (object should be previously
 * allocated).
 */
void
tc_png_load(memory_t* memory, texture_t* texture)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    unsigned int sig_read = 0, row_bytes = 0;
    int color_type, interlace_type;
    char filename[MAX_CHAR];
    png_uint_32 width, height;
    int bit_depth;
    png_bytepp row_pointers = NULL;
    unsigned int i;

    strncpy(filename, texture->name, MAX_CHAR);

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also supply the
     * the compiler header file version, so that we know if the application
     * was compiled with a compatible version of the library. REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);

    if (png_ptr == NULL) {
        log_e("Cannot create PNG pointer for: %s", filename);
        goto error;
    }

    /* Allocate/initialize the memory for image information. REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        log_e("Cannot create PNG information pointer for: %s", filename);
        goto error;
    }

    /* Set error handling if you are using the setjmp/longjmp method (this is
     * the normal method of doing things with libpng). REQUIRED unless you
     * set up your own error handlers in the png_create_read_struct() earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        log_e("Cannot set PNG error handling for: %s", filename);
        goto error;
    }

    if (!memory) {
        log_e("No memory raw data loaded from asset");
        goto error;
    }

    /* Set up our custom reading function useful for multi device loading. */
    png_set_read_fn(png_ptr, (void*) memory, read_memory);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /* Read all PNG info up to the image data */
    png_read_info(png_ptr, info_ptr);

    /* Get PNG bit_depth */
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    /* Get PNG color type */
    color_type = png_get_color_type(png_ptr, info_ptr);

    /* Now that we know more about the PNG we can set a few attributes. */
    if (color_type == PNG_COLOR_TYPE_PALETTE
    ||  png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)
    ||  (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8))
    {
        png_set_expand(png_ptr);
    }

    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY
    ||  color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(png_ptr);
    }

    /* Update that info to the struct and info pointers. */
    png_read_update_info(png_ptr, info_ptr);

    /* Get header info (width, height, bit depth, color type and interlace) */
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);

    /* Get bytes per row */
    row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    texture->width  = width;
    texture->height = height;

    switch (color_type) {
      case PNG_COLOR_TYPE_GRAY:
        texture->byte        = 1;
        texture->alpha       = 0;
        texture->ogl.iformat = GL_LUMINANCE;
        texture->ogl.format  = GL_LUMINANCE;
        break;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
        texture->byte        = 2;
        texture->alpha       = 1;
        texture->ogl.iformat = GL_LUMINANCE_ALPHA;
        texture->ogl.format  = GL_LUMINANCE_ALPHA;
        break;

      case PNG_COLOR_TYPE_RGB:
        texture->byte        = 3;
        texture->alpha       = 0;
        texture->ogl.iformat = GL_RGB;
        texture->ogl.format  = GL_RGB;
        break;

      case PNG_COLOR_TYPE_RGB_ALPHA:
        texture->byte        = 4;
        texture->alpha       = 1;
        texture->ogl.iformat = GL_RGBA;
        texture->ogl.format  = GL_RGBA;
        break;
    }

    /* Default OpenGL properties for PNG texture. */
    texture->ogl.tid    = 0;
    texture->ogl.target = GL_TEXTURE_2D;
    texture->ogl.type   = GL_UNSIGNED_BYTE;
    texture->ogl.unit   = 0;

    texture->nmipmap     = 0; /* PNG does not have mipmaps */
    texture->mipmapped   = 0; /* PNG does not have mipmaps */
    texture->compression = 0; /* PNG is loaded as RAW pixels */

    texture->size        = row_bytes * height;
    texture->pixels      = (unsigned char*) malloc(texture->size *
                                                   sizeof(unsigned char));
    if (!texture->pixels) {
        log_e("Not enough memory to create pixels for texture");
        goto error;
    }

    row_pointers = (png_bytepp) malloc(sizeof(png_bytepp) * height);
    if (!row_pointers) {
        log_e("Not enough memory to create row_pointers");
        goto error;
    }

    for (i = 0; i < height; i++) {
        row_pointers[height - i - 1] = (png_bytep) (texture->pixels) +
                                       i * row_bytes;
    }

    png_read_image(png_ptr, row_pointers);

    /* Clean up after the read, and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Free memory we don't need anymore */
    free(row_pointers);
    row_pointers = NULL;

    texture_print_info(texture);

    /* That's it */
    return;

error:
    if (png_ptr) {
        if (info_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        } else {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
        }
    }

    if (texture->pixels) {
        free(texture->pixels);
        texture->pixels = NULL;
    }

    if (row_pointers) {
        free(row_pointers);
        row_pointers = NULL;
    }
}

#endif
