/**
 * @file renderer.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"
#include "renderer.h"
/* TODO Perhaps remove dependency of scene.h */
#include "scene.h"

/**
 * Prints useful info about about OpenGL.
 *
 * This mactro converts #s into a proper string.
 */
#define print_gl_string(s) \
    LOGI("#s: %s", glGetString(s));

/**
 * Module structure.
 */
struct renderer_module_info {
    char* gl_extensions;
    char  tc_ext[4];
};

/**
 * Module static object.
 */
static struct renderer_module_info m = {
    "",
    ""
};

/**
 * Gets supported texture compression extension file.
 *
 * This is computed at runtime, when renderer is initialized.
 */
static void
device_get_tc_ext()
{
    if (renderer_has_gl_ext("EXT_texture_compression_s3tc")) {
        snprintf(m.tc_ext, 4, "dds");
    } else if (renderer_has_gl_ext("OES_compressed_ETC1_RGB8_texture")) {
        snprintf(m.tc_ext, 4, "ktx");
    } else {
        LOGE("no texture compression extension supported on this device");
    }
}

void
renderer_init()
{
    print_gl_string(GL_VERSION);
    print_gl_string(GL_VENDOR);
    print_gl_string(GL_RENDERER);
    print_gl_string(GL_EXTENSIONS);

    /* Getting OpenGL extensions. */
    m.gl_extensions = (char*) glGetString(GL_EXTENSIONS);

    /* Retrieve texture compression extension. */
    device_get_tc_ext();

    /* The following two lines enable semi transparent. */
    GL_CHECK(glEnable, GL_BLEND);
    GL_CHECK(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GL_CHECK(glDisable, GL_DEPTH_TEST);
    GL_CHECK(glDepthMask, GL_FALSE);
}

void
renderer_pre_render(float r, float g, float b, float a)
{
    GL_CHECK(glClearColor, r, g, b, a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void
renderer_render(scene_t* scene)
{
    static float time = 0;
    static int   frames = 0;

    float elapsed = main_get_time_elapsed(); /* time since last frame */

    time += main_get_time_elapsed();
    frames++;

    if (time >= 1.0f) {
        /* LOGI("FPS: %d", frames); */
        time = 0.0f;
        frames = 0;
    }

    scene_animate(scene, elapsed);

    renderer_pre_render(0.0f, 0.4f, 1.0f, 1.0f);
    scene_render(scene);
}

short
renderer_has_gl_ext(const char* ext)
{
    char*  curs;

    if (ext == NULL) {
        return 0;
    }

    /* Strip GL_ if present. */
    if (!strncmp(ext, "GL_", 3)) {
        ext += 3;
    }

    curs = strstr(m.gl_extensions, ext);

    return (curs != NULL);
}

char*
renderer_get_tc_ext()
{
    return m.tc_ext;
}
