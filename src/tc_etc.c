/**
 * Implements Ericsson Texture Compression.
 *
 * This file is compiled only if --enable-etc is activated when running
 * configure script (default).
 */

/**
 * @file tc_etc.c
 * @author Jeremy Attali
 */

#include "bina.h"

#ifdef ENABLE_ETC

#include "texture.h"
#include "ktx.h"

/* A few renames to make function calls consistent with our coding rules. */
#define ktx_load_texture_m ktxLoadTextureM
#define ktx_error_string   ktxErrorString

typedef KTX_dimensions ktx_dimensions_t;
typedef KTX_error_code ktx_error_code_t;

/**
 * Loads a KTX file into a texture object previously allocated.
 *
 * @param buffer The binary buffer that contains the KTX file data.
 * @param size The size of the buffer.
 * @param texture The texture object returned properly set up.
 */
void
etc_load_ktx(const unsigned char* buffer, unsigned int size,
              texture_t* texture)
{
    LOGD("loading ktx buffer with size: %d", size);

    const void*      bytes       = (const void*) buffer;
    bina_uint        tex         = 0;
    bina_enum        target      = 0;
    bina_boolean     mipmapped   = 0;
    bina_enum        glerror     = 0;
    bina_uint        kvdlen      = 0;
    bina_ubyte*      kvdpix      = NULL;
    ktx_dimensions_t dimensions;
    ktx_error_code_t res;
    const char*      strres;

    texture_ogl_t    ogl;

    res = ktx_load_texture_m(bytes, size, &tex, &target, &dimensions,
                             &mipmapped, &glerror, &kvdlen, &kvdpix);

    if (res != KTX_SUCCESS) {
        strres = ktx_error_string(res);
        LOGE("error while loading ktx file: %s", strres);
        goto error;
    } else {
        ogl.tid    = tex;
        ogl.target = target;
        ogl.unit   = 0;      /* TODO make sure there is always one unit */

        texture->width        = dimensions.width;
        texture->height       = dimensions.height;
        texture->compression  = GL_TRUE;
        texture->flags       |= TEXTURE_UPLOADED | TEXTURE_FLIP_VERTICAL;
        texture->ogl          = ogl;
    }

    return;

error:

    if (texture->pixels) {
        free(texture->pixels);
        texture->pixels = NULL;
    }
}

#endif
