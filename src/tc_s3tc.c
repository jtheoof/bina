/**
 * Implements S3 (DX) Texture Compression.
 *
 * This file is compiled only if --enable-s3tc is activated when running
 * configure script.
 */

/**
 * @file tc_s3tc.c
 * @author Jeremy Attali, Johan Attali
 * @date August 24, 2013
 */

#include "bina.h"
#include "texture.h"

#ifdef ENABLE_S3TC

#define DDS_MAGIC "DDS "

#define DDSD_CAPS        0x1
#define DDSD_HEIGHT      0x2
#define DDSD_WIDTH       0x4
#define DDSD_PITCH       0x8
#define DDSD_PIXELFORMAT 0x1000
#define DDSD_MIPMAPCOUNT 0x20000
#define DDSD_LINEARSIZE  0x80000
#define DDSD_DEPTH       0x800000

#define DDSCAPS_COMPLEX 0x8
#define DDSCAPS_MIPMAP  0x400000
#define DDSCAPS_TEXTURE 0x1000

#define DDSCAPS2_CUBEMAP           0x200
#define DDSCAPS2_CUBEMAP_POSITIVEX 0x400
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x800
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x1000
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x2000
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x4000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x8000
#define DDSCAPS2_VOLUME            0x200000

#define DDPF_ALPHAPIXELS 0x1
#define DDPF_ALPHA       0x2
#define DDPF_FOURCC      0x4
#define DDPF_RGB         0x40
#define DDPF_YUV         0x200
#define DDPF_LUMINANCE   0x20000

typedef struct dds_pixel_format_t {
    unsigned int size;
    unsigned int flags;
    char         four_cc[4];
    unsigned int rgb_bit_count;
    unsigned int r_bit_mask;
    unsigned int g_bit_mask;
    unsigned int b_bit_mask;
    unsigned int a_bit_mask;
} dds_pixel_format_t;

typedef struct dds_header_t {
    unsigned int       size;
    unsigned int       flags;
    unsigned int       height;
    unsigned int       width;
    unsigned int       pitch_or_linear_size;
    unsigned int       depth;
    unsigned int       mipmap_count;
    unsigned int       reserved1[11];
    dds_pixel_format_t ddspf;
    unsigned int       caps;
    unsigned int       caps2;
    unsigned int       caps3;
    unsigned int       caps4;
    unsigned int       reserved2;
} dds_header_t;

/**
 * Checks the presence of the s3tc extension in current hardware.
 *
 * @return 0 if hardware does not support EXT_texture_compression_s3tc.
 */
short
s3tc_check_extension()
{
    const char* exts = (const char*) glGetString(GL_EXTENSIONS);
    const char* s3tc = "EXT_texture_compression_s3tc"; 
    const char* curs = strstr(exts, s3tc);

    return (curs != NULL);
}


static short
s3tc_dds_read_header(const unsigned char* buffer, unsigned int size,
                     dds_header_t** dds_header, unsigned int* offset)
{
    unsigned int   caps;
    unsigned int   remains = size;

    dds_header_t* header;
    dds_pixel_format_t ddspf;

    if (size < 4) {
        LOGE("invalid dds file - size is too small");
        return GL_INVALID_VALUE;
    }

    if (strncmp((char *) buffer, DDS_MAGIC, 4)) {
        LOGE("invalid dds file - missing magic");
        return GL_INVALID_VALUE;
    }

    remains -= 4;
    buffer  += 4;

    header = (dds_header_t *) buffer;
    ddspf  = header->ddspf;

    if (header->size != sizeof(dds_header_t)) {
        LOGE("invalid dds file - invalid header size");
        return GL_INVALID_VALUE;
    }
    if (header->ddspf.size != sizeof(dds_pixel_format_t)) {
        LOGE("invalid dds file - invalid pixel format size");
        return GL_INVALID_VALUE;
    }

    remains -= sizeof(dds_header_t);
    buffer  += sizeof(dds_header_t);

    if (ddspf.flags & DDPF_RGB) {
        switch (ddspf.rgb_bit_count) {
          default:
            LOGE("invalid dds file - unsupported compressed format");
            return GL_INVALID_VALUE;
        }
    }

    /* Get DDS file capabilities. */
    caps  = header->caps;

    /* Check to see if texture has 'magic' DXTn. */
    if (ddspf.flags & DDPF_FOURCC) {
        switch (ddspf.four_cc[3]) {
          case '1':
          case '3':
          case '5':
            break;
          default:
            LOGE("invalid dds file - %s is not supported", ddspf.four_cc);
            return GL_INVALID_VALUE;
        }
    } else {
        LOGE("unsupported dds flags: 0x%08X", ddspf.flags);
        return GL_INVALID_VALUE;
    }

    if (caps & DDSCAPS_COMPLEX) {
        LOGE("unsupported dds caps: DDSCAPS_COMPLEX");
        return GL_INVALID_VALUE;
    }
    if (caps & DDSCAPS_MIPMAP) {
        LOGE("unsupported dds caps: DDSCAPS_MIPMAP");
        return GL_INVALID_VALUE;
    }
    if (!caps & DDSCAPS_TEXTURE) {
        LOGE("invalid dds file - dds caps should have DDSCAPS_TEXTURE");
        return GL_INVALID_VALUE;
    }

    *dds_header = header;
    *offset     = size - remains;

    return 0;
}

/**
 * Loads a DDS file into a texture object previously allocated.
 *
 * @param buffer The binary buffer that contains the DDS file data.
 * @param size The size of the buffer.
 * @param texture The texture object returned properly set up.
 */
void
s3tc_dds_load(const unsigned char* buffer, unsigned int size,
              texture_t* texture)
{
    LOGD("loading dds buffer with size: %d", size);

    unsigned short alpha = 0;
    unsigned int   nmipmap, bufsize, blksize, i;
    unsigned long  width, height;

    unsigned int       offset;
    dds_header_t*      header;
    dds_pixel_format_t ddspf;

    if (s3tc_dds_read_header(buffer, size, &header, &offset)) return;

    ddspf = header->ddspf;

    /* Check to see if texture has alpha. */
    alpha = (ddspf.flags & DDPF_ALPHAPIXELS || ddspf.flags & DDPF_ALPHA);

    /* Check to see if texture has 'magic' DXTn. */
    if (ddspf.flags & DDPF_FOURCC) {
        switch (ddspf.four_cc[3]) {
          case '1':
            if (alpha) {
                texture->ogl.iformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            } else {
                texture->ogl.iformat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
            }
            blksize = 8;
            break;
          case '3':
            texture->ogl.iformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            if (!alpha) {
                LOGE("watch out!! alpha should be 1!!!");
            }
            blksize = 16;
            break;
          case '5':
            texture->ogl.iformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            if (!alpha) {
                LOGE("watch out!! alpha should be 1!!!");
            }
            blksize = 16;
            break;
          default:
            break;
        }
    }

    width   = header->width;
    height  = header->height;
    nmipmap = header->mipmap_count;
    bufsize = 0;

    /* Force at least one mipmap for proper texture upload to GPU. */
    if (nmipmap == 0) {
        nmipmap = 1;
    }

    for (i = 0; (i < nmipmap) && ((width != 0) || (height != 0)); i++) {
        bufsize += ((width + 3) / 4) * ((height + 3) / 4) * blksize;

        if ((i == 0) && (bufsize != header->pitch_or_linear_size)) {
            LOGE("buffer size in invalid for first mipmap");
        }

        if ((width /= 2) == 0) width = 1;
        if ((height /= 2) == 0) height = 1;
    }

    /* Header has been parsed, allocating memory for rest of buffer. */
    texture->pixels = (unsigned char*) malloc(bufsize * sizeof(unsigned char));

    if (!texture->pixels) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

    memcpy(texture->pixels, &buffer[offset], bufsize);

    /* Default OpenGL properties for DDS texture. */
    texture->ogl.tid    = 0;
    texture->ogl.target = GL_TEXTURE_2D; /* same as most file format */
    texture->ogl.type   = 0;             /* do not care about texel type */
    texture->ogl.unit   = 0;

    texture->width       = header->width;
    texture->height      = header->height;
    texture->size        = bufsize;
    texture->alpha       = alpha;
    texture->compression = 1;                     /* compressed format */
    texture->blksize     = blksize;               /* compression block size */
    texture->nmipmap     = nmipmap;
    texture->flags       = TEXTURE_FLIP_VERTICAL; /* flip before render */

    return;

error:

    if (texture->pixels) {
        free(texture->pixels);
        texture->pixels = NULL;
    }
}

#endif
