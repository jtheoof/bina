#include "bina.h"

#ifdef HAVE_PNG_H

static short
le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}

/* TODO Get last error codes */

/* TODO OpenGL external format must be: GL_BGR
 *      OpenGL internal format must be: GL_RGB8
 */
int
texture_load_png(const char* filename,
                 int* width, int* height, int* alpha,
                 void **pixels)
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
 
    LOGD("Loading: %s", filename);

    if ((fp = fopen(filename, "rb")) == NULL) {
        LOGE("Unable to open: %s for reading", filename);
        return 1;
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
        fclose(fp);
        return 1;
    }
 
    /* Allocate/initialize the memory for image information. REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        LOGE("Cannot create PNG information pointer for: %s", filename);
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 1;
    }
 
    /* Set error handling if you are using the setjmp/longjmp method (this is
     * the normal method of doing things with libpng). REQUIRED unless you
     * set up your own error handlers in the png_create_read_struct() earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("Cannot set PNG error handling for: %s", filename);
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return 1;
    }
 
    /* Set up the output control if you are using standard C streams */
    png_init_io(png_ptr, fp);
 
    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);
 
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
    png_read_png(png_ptr, info_ptr,
                 PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING |
                 PNG_TRANSFORM_EXPAND, NULL);
 
    png_uint_32 w, h;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);

    *width = w;
    *height = h;
    *alpha = color_type == PNG_COLOR_TYPE_RGB_ALPHA;
 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *pixels = malloc(row_bytes * h);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (int i = 0; i < h; i++) {
        /* Note that png is ordered top to bottom, but OpenGL expect it bottom
         * to top so the order or swapped */
        memcpy(*pixels+(row_bytes * (h-1-i)),
               row_pointers[i], row_bytes);
    }

    /* Clean up after the read, and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
 
    /* Close the file */
    fclose(fp);

    LOGI("Loaded: %s width: %d height: %d alpha: %d",
         filename, *width, *height, *alpha);
 
    /* That's it */
    return 0;
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

    LOGD("Loading: %s", filename);

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
