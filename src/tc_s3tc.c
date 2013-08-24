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

typedef struct dds_pixel_format_t {
    unsigned int size;
    unsigned int flags;
    unsigned int four_cc;
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

void
s3tc_dds_load(const unsigned char* buffer, unsigned int size,
              texture_t* texture)
{
    LOGD("Loading buffer with size: %d", size);

    unsigned long level_size, width, height, pitch;
    dds_header_t* header;

    if (size < 4) {
        LOGE("invalid dds file - size is too small");
        goto error;
    }
    if (strncmp((char *) buffer, DDS_MAGIC, 4)) {
        LOGE("invalid dds file - missing magic");
        goto error;
    }

    size -= 4;
    buffer += 4;

    header = (dds_header_t *) buffer;

    if (header->size != sizeof(dds_header_t)) {
        LOGE("invalid dds file - invalid header size");
    }
    if (header->ddspf.size != sizeof(dds_pixel_format_t)) {
        LOGE("invalid dds file - invalid pixel format size");
    }

    size -= sizeof(dds_header_t);
    buffer += sizeof(dds_header_t);

    height = header->height;
    width  = header->width;

    pitch = 0;
    level_size = 0;

    /* if (header->ddspf.flags & 0x00000040) { */
    /*     switch(header->ddspf.rgb_bit_count) { */
    /*         case 32: */
    /*             pitch = 4 * header->width; */
    /*             /1* dds->format = URGBA32; *1/ */
    /*             break; */
    /*         default: */
    /*             LOGE("invalid dds file - unsupported uncompressed format"); */
    /*     } */
    /* } else if (header->ddspf.flags & 0x00000004) { */
    /*     switch (header->ddspf.four_cc[3]) { */
    /*         case '1': */
    /*             /1* dds->format = DXT1; *1/ */
    /*             break; */
    /*         case '2': */
    /*             /1* dds->format = DXT2; *1/ */
    /*             break; */
    /*         case '3': */
    /*             /1* dds->format = DXT3; *1/ */
    /*             break; */
    /*         case '4': */
    /*             /1* dds->format = DXT4; *1/ */
    /*             break; */
    /*         case '5': */
    /*             /1* dds->format = DXT5; *1/ */
    /*             break; */
    /*         default: */
    /*             LOGE("invalid dds file - unsupported dds format"); */
    /*             break; */
    /*     } */
    /* } else { */
    /*     LOGE("unsupported dds flags: 0x%08X", header->ddspf.flags); */
    /* } */

    return;

error:
    return;
}

#endif
