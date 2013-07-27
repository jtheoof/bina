/**
 * @file texture.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

#ifdef HAVE_PNG_H

/* Static functions */
static short
le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}

/* TODO Get last error codes */

/* TODO This should return the texture itself, not an int. */

int
texture_load_png(texture_t* texture)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    unsigned int sig_read = 0, row_bytes = 0;
    int color_type, interlace_type;
    FILE *fd;
    char filename[MAX_CHAR];
    png_uint_32 width, height;
    int bit_depth;
    png_bytepp row_pointers = NULL;
    unsigned int i;

    strncpy(filename, texture->name, MAX_CHAR);

    if ((fd = fopen(filename, "rb")) == NULL) {
        LOGE("Unable to open: %s for reading", filename);
        goto error;
    }

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also supply the
     * the compiler header file version, so that we know if the application
     * was compiled with a compatible version of the library. REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);

    if (png_ptr == NULL) {
        LOGE("Cannot create PNG pointer for: %s", filename);
        goto error;
    }

    /* Allocate/initialize the memory for image information. REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        LOGE("Cannot create PNG information pointer for: %s", filename);
        goto error;
    }

    /* Set error handling if you are using the setjmp/longjmp method (this is
     * the normal method of doing things with libpng). REQUIRED unless you
     * set up your own error handlers in the png_create_read_struct() earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("Cannot set PNG error handling for: %s", filename);
        goto error;
    }

    /* Set up the output control if you are using standard C streams */
    png_init_io(png_ptr, fd);

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

    /*
     * If you have enough memory to read in the entire image at once, and you
     * need to specify only transforms that can be controlled with one of the
     * PNG_TRANSFORM_* bits (this presently excludes dithering, filling,
     * setting background, and doing gamma adjustment), then you can read the
     * entire image (including pixels) into the info structure with this call.
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    /* png_read_png(png_ptr, info_ptr, */
    /*              PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | */
    /*              PNG_TRANSFORM_EXPAND, NULL); */

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
        texture->byte    = 1;
        texture->iformat = GL_LUMINANCE;
        texture->format  = GL_LUMINANCE;
        texture->alpha   = 0;
        break;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
        texture->byte    = 2;
        texture->iformat = GL_LUMINANCE_ALPHA;
        texture->format	 = GL_LUMINANCE_ALPHA;
        texture->alpha   = 1;
        break;

      case PNG_COLOR_TYPE_RGB:
        texture->byte	 = 3;
        texture->iformat = GL_RGB;
        texture->format	 = GL_RGB;
        texture->alpha   = 0;
        break;

      case PNG_COLOR_TYPE_RGB_ALPHA:
        texture->byte    = 4;
        texture->iformat = GL_RGBA;
        texture->format	 = GL_RGBA;
        texture->alpha   = 1;
        break;
    }

    texture->type = GL_UNSIGNED_BYTE;

    texture->pixels = malloc(row_bytes * height);
    if (!texture->pixels) {
        LOGE("Not enough memory to create pixels for texture");
        goto error;
    }

    row_pointers = (png_bytepp) malloc(sizeof(png_bytepp) * height);
    if (!row_pointers) {
        LOGE("Not enought memory to create row_pointers");
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

    /* Close the file */
    fclose(fd);

    texture_print_info(texture);

    /* That's it */
    return 0;

error:
    if (png_ptr) {
        if (info_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        } else {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
        }
    }

    if (fd) {
        fclose(fd);
    }

    if (texture->pixels) {
        free(texture->pixels);
        texture->pixels = NULL;
    }

    if (row_pointers) {
        free(row_pointers);
        row_pointers = NULL;
    }

    return 1;
}

/* TODO OpenGL external format must be: GL_BGR
 *      OpenGL internal format must be: GL_RGB8
 */
int
texture_load_tga(const char *filename,
                 int *width, int *height, void** pixels)
{
    struct tga_header {
       char  id_length;
       char  color_map_type;
       char  data_type_code;
       unsigned char  color_map_origin[2];
       unsigned char  color_map_length[2];
       char  color_map_depth;
       unsigned char  x_origin[2];
       unsigned char  y_origin[2];
       unsigned char  width[2];
       unsigned char  height[2];
       char  bits_per_pixel;
       char  image_descriptor;
    } header;

    int i, color_map_size, pixels_size;
    FILE *f;
    size_t read;

    f = fopen(filename, "rb");

    if (!f) {
        LOGE("Unable to open: %s for reading", filename);
        return 1;
    }

    read = fread(&header, 1, sizeof(header), f);

    if (read != sizeof(header)) {
        LOGE("%s has incomplete tga header", filename);
        fclose(f);
        return 1;
    }
    if (header.data_type_code != 2) {
        LOGE("%s is not an uncompressed RGB tga file",
             filename);
        fclose(f);
        return 1;
    }
    if (header.bits_per_pixel != 24) {
        LOGE("%s is not a 24-bit uncompressed RGB tga file",
             filename);
        fclose(f);
        return 1;
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF) {
            LOGE("%s has incomplete id string", filename);
            fclose(f);
            return 1;
        }

    color_map_size = le_short(header.color_map_length) *
                     (header.color_map_depth / 8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF) {
            LOGE("%s has incomplete color map", filename);
            fclose(f);
            return 1;
        }

    *width = le_short(header.width); *height = le_short(header.height);
    pixels_size = *width * *height * (header.bits_per_pixel / 8);
    *pixels = malloc(pixels_size);

    read = fread(*pixels, 1, pixels_size, f);
    fclose(f);

    if (read != pixels_size) {
        LOGE("%s has incomplete image", filename);
        free(*pixels);
        return 1;
    }

    LOGD("Loaded: %s width: %d height: %d", filename, *width, *height);

    return 0;
}

#endif /* HAVE_PNG_H */

texture_t*
texture_create(const char* name)
{
    texture_t* texture;

    texture = (texture_t*) malloc(sizeof(texture_t));

    if (!texture) {
        LOGE("Not enough memory to create texture: %s", name);
        goto error;
    }

    texture->id     = 0;
    texture->pixels = NULL;

    strncpy(texture->name, name, MAX_CHAR);
    if (texture_load(texture)) {
        LOGE("An error occured while loading the texture");
        goto error;
    }

    /* TODO Make this more flexible */
    texture->target  = GL_TEXTURE_2D;
    texture->iformat = texture->alpha ? GL_RGBA : GL_RGB;
    texture->format  = texture->iformat;
    texture->unit    = 0;

    texture_gl_create(texture);

    return texture;

error:
    texture_delete(&texture);
    return NULL;
}

void
texture_delete(texture_t** texture)
{
    texture_t* tex = *texture;

    if (tex) {
        texture_gl_delete(tex);
        if (tex->pixels) {
            free(tex->pixels);
            tex->pixels = NULL;
        }
        free(tex);
        tex = NULL;

        *texture = tex;
    }
}

int
texture_load(texture_t* texture)
{

    char ext[MAX_CHAR];
    int err;
    get_file_extension(texture->name, ext);

    LOGD("Loading: %s", texture->name);

    if (!strcmp(ext, "png")) {
        err = texture_load_png(texture);
    } else if (!strcmp(ext, "tga")) {
        LOGE("Extension tga need some work with texture");
    } else {
        LOGE("Extension: %s not implemented for texturing", ext);
    }

    return err;
}

int
texture_gl_create(texture_t* texture)
{
    int err;

    if (texture->id) {
        err = texture_gl_delete(texture);
        if (err) {
            return err;
        }
    }

    GL_CHECK(glGenTextures, 1, &texture->id);
    LOGD("Texture has id: %d", texture->id);

    GL_CHECK(glBindTexture, texture->target, texture->id);

	switch (texture->byte) {
		case 1:
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            break;
		case 2:
            glPixelStorei(GL_PACK_ALIGNMENT, 2);
            break;
		case 3:
		case 4:
            glPixelStorei(GL_PACK_ALIGNMENT, 4);
            break;
    }

    GL_CHECK(glTexParameteri, texture->target,
             GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL_CHECK(glTexParameteri, texture->target,
             GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL_CHECK(glTexParameteri, texture->target,
             GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GL_CHECK(glTexParameteri, texture->target,
             GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GL_CHECK(glTexImage2D, texture->target, 0,
             texture->iformat,
             texture->width, texture->height, 0,
             texture->format, texture->type,
             texture->pixels);

    GL_CHECK(glBindTexture, texture->target, 0);

    return 0;
}

int
texture_gl_delete(texture_t* texture)
{
    if (texture->id) {
        GL_CHECK(glDeleteTextures, 1, &texture->id);
        texture->id;
    }

    return 0;
}

void
texture_print_info(texture_t* texture)
{
    LOGI("Loaded: %s width: %d height: %d alpha: %d",
         texture->name, texture->width, texture->height, texture->alpha);
}
