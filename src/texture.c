/**
 * @file texture.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"
#include "texture.h"
#include "utils.h"

#ifdef HAVE_PNG_H

/* Static functions */
static short
le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}

void
texture_png_read_memory(png_structp pngp, png_bytep bytep, png_size_t size)
{

    memory_t* m = (memory_t*) png_get_io_ptr(pngp);

    memory_read(m, bytep, size);
}

int
texture_png_load(memory_t* memory, texture_t* texture)
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

    if (!memory) {
        LOGE("No memory raw data loaded from asset");
        goto error;
    }

    /* Set up our custom reading function useful for multi device loading. */
    png_set_read_fn(png_ptr, (void*) memory, texture_png_read_memory);

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
    texture->compression = 0; /* PNG is loaded as RAW pixels */

    texture->size        = row_bytes * height;
    texture->pixels      = (unsigned char*) malloc(texture->size *
                                                   sizeof(unsigned char));
    if (!texture->pixels) {
        LOGE("Not enough memory to create pixels for texture");
        goto error;
    }

    row_pointers = (png_bytepp) malloc(sizeof(png_bytepp) * height);
    if (!row_pointers) {
        LOGE("Not enough memory to create row_pointers");
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
    return 0;

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

    return 1;
}

#endif /* HAVE_PNG_H */

void
texture_dds_load(memory_t* memory, texture_t* texture)
{
#ifdef ENABLE_S3TC
#include "tc_s3tc.h"
    static short s3tc_check = -1;

    if (s3tc_check == -1) {
        s3tc_check = s3tc_check_extension();

        if (!s3tc_check) {
            LOGE("s3tc is not supported on this hardware");
        }
    }

    if (s3tc_check) {
        s3tc_dds_load(memory->buffer, memory->size, texture);
    }
#else
    LOGE("s3tc support must be enabled to load dds files");
#endif
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

    LOGD("tga: %s loaded - width: %d height: %d", filename, *width, *height);

    return 0;
}

texture_t*
texture_create(const char* name, unsigned long flags)
{
    texture_t* texture;

    texture = (texture_t*) calloc(1, sizeof(texture_t));

    if (!texture) {
        LOGE("not enough memory to create texture: %s", name);
        goto error;
    }

    strncpy(texture->name, name, MAX_CHAR);

    texture->pixels = NULL;
    texture->flags  = flags; /* init flags but can be auto updated later */

    if (texture_load(name, texture)) {
        LOGE("an error occured while loading the texture");
        goto error;
    }

    texture_gl_create(texture);

    if ((flags & TEXTURE_KEEP_IN_MEMORY) && texture->pixels) {
        free(texture->pixels);
        texture->pixels = NULL;
    }

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

        if (tex->image) {
            memory_delete(&tex->image);
        }

        free(tex);
        tex = NULL;

        *texture = tex;
    }
}

texture_list_t*
texture_create_list(const char* folder,
                    const char* animation,
                    const char* ext,
                    const unsigned short size)
{
    texture_list_t* ret = NULL;
    char  tmp[MAX_PATH];
    int   i;

    ret = (texture_list_t*) calloc(1, sizeof(texture_list_t));
    if (!ret) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

    ret->size = size;
    ret->tid  = 0;
    ret->textures = (texture_t**) calloc(size, sizeof(texture_t*));

    if (!ret->textures) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

    for (i = 0; i < size; i++) {
        snprintf(tmp, MAX_PATH, "%s/%s/%s_%03d.%s",
                 "animations", folder, animation, i, ext);
        ret->textures[i] = texture_create(tmp, 0);
    }

    return ret;

error:
    texture_delete_list(&ret);

    return NULL;
}

void
texture_delete_list(texture_list_t** list)
{
    texture_list_t* tmp = *list;
    int i;

    if (tmp) {
        if (tmp->textures) {
            for (i = 0; i < tmp->size; i++) {
                texture_delete(&tmp->textures[i]);
            }
            free(tmp->textures);
            tmp->textures = NULL;
        }
        free(tmp);
        *list = NULL;
    }
}

int
texture_load(const char* name, texture_t* texture)
{
    memory_t* memory = NULL;

    char ext[MAX_CHAR] = "";
    int  err = 0;

    if (!texture) {
        return 0;
    }

    get_file_extension(name, ext);

    /* Load asset in memory */
    memory = memory_create(name);
    if (!memory) {
        LOGE("object: %s was not loaded into memory", name);
        texture->image = NULL;
        return -1;
    }

    if (!strcmp(ext, "png")) {
        err = texture_png_load(memory, texture);
    } else if (!strcmp(ext, "tga")) {
        LOGE("Extension tga need some work with texture");
        memory_delete(&memory);
    } else if (!strcmp(ext, "dds")) {
        texture_dds_load(memory, texture);
    } else {
        LOGE("Extension: %s not implemented for texturing", ext);
        memory_delete(&memory);
    }

    /* The object has been loaded in memory and put into texture->pixels so
     * there is no need for previous original data loaded in memory object.
     * Unless, of course, the caller specifically asked to keep it (scale map
     * for example).
     */
    if (!(texture->flags & TEXTURE_KEEP_IN_MEMORY)) {
        memory_delete(&memory);
        texture->image = NULL;
    } else {
        texture->image = memory;
    }

    return err;
}

/* TODO Add custom flags and filters like it's done in gfx */
int
texture_gl_create(texture_t* texture)
{
    int err;

    if (texture->ogl.tid) {
        err = texture_gl_delete(texture);
        if (err) {
            return err;
        }
    }

    GL_CHECK(glGenTextures, 1, &texture->ogl.tid);
    LOGD("Texture has id: %d", texture->ogl.tid);

    GL_CHECK(glBindTexture, texture->ogl.target, texture->ogl.tid);

    if (!texture->compression) {
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
    }

    GL_CHECK(glTexParameteri, texture->ogl.target,
             GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL_CHECK(glTexParameteri, texture->ogl.target,
             GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL_CHECK(glTexParameteri, texture->ogl.target,
             GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GL_CHECK(glTexParameteri, texture->ogl.target,
             GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (texture->compression) {
        unsigned int i      = 0,
                     width  = texture->width,
                     height = texture->height,
                     bsize  = texture->blksize,
                     size   = 0,
                     offset = 0;

        if (!texture->nmipmap) {
            LOGE("texture: %d has missing mipmaps", texture->ogl.tid);
        } else {
            for (i = 0; i < texture->nmipmap; i++) {
                size = ((width + 3) >> 2) * ((height + 3) >> 2) * bsize;

                GL_CHECK(glCompressedTexImage2D, texture->ogl.target, i,
                         texture->ogl.iformat, width, height, 0, size,
                         &texture->pixels[offset]);

                if ((width  >>= 1) == 0) width  = 1;
                if ((height >>= 1) == 0) height = 1;

                offset += size;
            }
        }
    } else {
        GL_CHECK(glTexImage2D, texture->ogl.target, 0,
                 texture->ogl.iformat,
                 texture->width, texture->height, 0,
                 texture->ogl.format, texture->ogl.type,
                 texture->pixels);
    }

    GL_CHECK(glBindTexture, texture->ogl.target, 0);

    return 0;
}

int
texture_gl_delete(texture_t* texture)
{
    if (texture->ogl.tid) {
        GL_CHECK(glDeleteTextures, 1, &texture->ogl.tid);
    }

    return 0;
}

void
texture_print_info(texture_t* texture)
{
    LOGI("Loaded: %s width: %d height: %d alpha: %d",
         texture->name, texture->width, texture->height, texture->alpha);
}
