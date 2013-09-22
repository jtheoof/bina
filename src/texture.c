/**
 * @file texture.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"
#include "texture.h"
#include "utils.h"
#include "renderer.h"

static short
le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}

void
texture_png_load(memory_t* memory, texture_t* texture)
{
#ifdef ENABLE_PNG
#include "tc_png.h"
    tc_png_load(memory, texture);
#else
    LOGE("png support must be enabled to load png files");
#endif
}

void
texture_dds_load(memory_t* memory, texture_t* texture)
{
#ifdef ENABLE_S3TC
#include "tc_s3tc.h"
    static short s3tc_check = -1;

    if (s3tc_check == -1) {
        s3tc_check = renderer_has_gl_ext("EXT_texture_compression_s3tc");

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
        texture_png_load(memory, texture);
    } else if (!strcmp(ext, "tga")) {
        LOGE("extension tga need some work with texture");
        memory_delete(&memory);
    } else if (!strcmp(ext, "dds")) {
        texture_dds_load(memory, texture);
    } else {
        LOGE("extension: %s not implemented for texturing", ext);
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
