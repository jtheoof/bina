/**
 * @file texture.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"
#include "texture.h"
#include "log.h"
#include "utils.h"
#include "renderer.h"
#include "error.h"
#include "utils.h"

/**
 * OpenGL helper function to free the memory used by the texture in the GPU.
 */
static void
texture_gl_delete(texture_t* texture)
{
    if (texture->ogl.tid) {
        GL_CHECK(glDeleteTextures, 1, &texture->ogl.tid);
    }
}

/**
 * OpenGL helper function to create a new texture buffer and apply it to the
 * pixels raw data.
 *
 * TODO:
 *  - Add custom flags and filters like it's done in gfx
 *  - Every texture loading function should be responsible for creating
 *  calling the specific OpenGL callbacks as it is done in loader.c (libktx).
 *
 * @param texture The texture to upload to the GPU.
 */
static int
texture_upload(texture_t* texture)
{
    if (texture->ogl.tid) {
        texture_gl_delete(texture);
    }

    GL_CHECK(glGenTextures, 1, &texture->ogl.tid);
    log_d("Texture has id: %d", texture->ogl.tid);

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

    if (texture->compression) {
        unsigned int i      = 0,
                     width  = texture->width,
                     height = texture->height,
                     bsize  = texture->blksize,
                     size   = 0,
                     offset = 0;

        if (!texture->nmipmap) {
            log_e("texture: %d has missing mipmaps", texture->ogl.tid);
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


/**
 * Update texture parameters such as filtering, wrapping, etc.
 *
 * The function is based on the texture flags value and also the filter valued
 * passed to the function.
 *
 * @param texture The texture to update.
 * @param filter The filter to apply, see: #texture_filter_enum_t.
 *
 * For example:
 *     texture_set_filter(mytex, TEXTURE_FILTER_1X);
 */
static void
texture_update_params(texture_t* texture, bina_enum filter)
{
    bina_uint target = texture->ogl.target;
    bina_uint tid    = texture->ogl.tid;
    bina_uint flags  = texture->flags;

    GL_CHECK(glBindTexture, target, tid);

    if (flags & TEXTURE_WRAP_REPEAT) {
        GL_CHECK(glTexParameteri, target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        GL_CHECK(glTexParameteri, target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        GL_CHECK(glTexParameteri, target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GL_CHECK(glTexParameteri, target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }


    if (flags & TEXTURE_MIPMAP) {
        switch (filter) {
          case TEXTURE_FILTER_1X:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;

          case TEXTURE_FILTER_2X:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;

          case TEXTURE_FILTER_3X:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;

          case TEXTURE_FILTER_0X:
          default:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        }
    } else {
        switch (filter) {
          case TEXTURE_FILTER_0X:
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
          default:
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        }
    }

    GL_CHECK(glBindTexture, target, 0);
}

void
texture_load_png(memory_t* memory, texture_t* texture)
{
#ifdef ENABLE_PNG
#include "tc_png.h"
    tc_png_load(memory, texture);
#else
    log_e("png support must be enabled to load png files");
#endif
}

void
texture_load_dds(memory_t* memory, texture_t* texture)
{
#ifdef ENABLE_S3TC
#include "tc_s3tc.h"
    static short ext_check = -1;

    if (ext_check == -1) {
        ext_check = renderer_has_gl_ext("EXT_texture_compression_s3tc");

        if (!ext_check) {
            log_e("s3tc is not supported on this hardware");
        }
    }

    if (ext_check) {
        s3tc_load_dds(memory->buffer, memory->size, texture);
    }
#else
    log_e("s3tc support must be enabled to load dds files");
#endif
}

void
texture_load_ktx(memory_t* memory, texture_t* texture)
{
#ifdef ENABLE_ETC
#include "tc_etc.h"
    static short ext_check = -1;

    if (ext_check == -1) {
        ext_check = renderer_has_gl_ext("OES_compressed_ETC1_RGB8_texture");

        if (!ext_check) {
            log_e("etc is not supported on this hardware");
        }
    }

    if (ext_check) {
        etc_load_ktx(memory->buffer, memory->size, texture);
    }
#else
    log_e("etc support must be enabled to load ktx files");
#endif
}

texture_t*
texture_create(const char* name, bina_enum flags, bina_enum filter)
{
    texture_t* texture;

    texture = (texture_t*) calloc(1, sizeof(texture_t));

    if (!texture) {
        log_e("not enough memory to create texture: %s", name);
        goto error;
    }

    strncpy(texture->name, name, MAX_CHAR);

    texture->pixels = NULL;
    texture->flags  = flags; /* init flags but can be auto updated later */

    if (texture_load(name, texture)) {
        log_e("an error occured while loading the texture");
        goto error;
    }

    if (!(texture->flags & TEXTURE_UPLOADED)) {
        texture_upload(texture);
    }

    /*
     * The texture has been uploaded to there is a valid texture id.
     * Let's update its parameters.
     */
    texture_update_params(texture, filter);

    if (!(texture->flags & TEXTURE_KEEP_PIXELS) && texture->pixels) {
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
        log_e(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

    ret->size = size;
    ret->tid  = 0;
    ret->textures = (texture_t**) calloc(size, sizeof(texture_t*));

    if (!ret->textures) {
        log_e(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

    for (i = 0; i < size; i++) {
        snprintf(tmp, MAX_PATH, "%s/%s/%s_%03d.%s",
                 "animations", folder, animation, i, ext);
        ret->textures[i] = texture_create(tmp, 0, TEXTURE_FILTER_1X);
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
        log_e("object: %s was not loaded into memory", name);
        return -1;
    }

    if (!strcmp(ext, "png")) {
        texture_load_png(memory, texture);
    } else if (!strcmp(ext, "dds")) {
        texture_load_dds(memory, texture);
    } else if (!strcmp(ext, "ktx")) {
        texture_load_ktx(memory, texture);
    } else {
        log_e("extension: %s not implemented for texturing", ext);
        memory_delete(&memory);
    }

    /* The object has been loaded in memory and put into texture->pixels so
     * there is no need for previous original data loaded in memory object,
     * even for scale maps. Scale maps only use texture->pixels directly.
     */
    memory_delete(&memory);

    return err;
}

void
texture_print_info(texture_t* texture)
{
    log_i("Loaded: %s width: %d height: %d alpha: %d",
         texture->name, texture->width, texture->height, texture->alpha);
}
