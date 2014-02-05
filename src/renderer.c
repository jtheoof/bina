/**
 * @file renderer.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "main.h"

#ifdef HAVE_GL_GLEW_H
#include <GL/glew.h>
#endif

#include "renderer.h"
#include "log.h"
#include "error.h"

/**
 * Prints useful info about about OpenGL.
 *
 * This mactro converts #s into a proper string.
 */
#define print_gl_string(s) \
    log_i(#s ": %s", glGetString(s));

/**
 * Module structure.
 */
struct renderer_module_info {
    sdl_window_t* window;      /* SDL window (previously initalized) */
    sdl_renderer_t* renderer;  /* SDL renderer */
    sdl_gl_context_t* context; /* SDL OpenGL context */
    char* gl_extensions;       /* opengl extensions */
    char  tc_ext[4];           /* texture compression extension */
};

/**
 * Module static object.
 */
static struct renderer_module_info m = {
    NULL,
    NULL,
    NULL,
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
    if (renderer_has_gl_ext("EXT_texture_compression_s3tc")
    ||  renderer_has_gl_ext("S3_s3tc"))
    {
        snprintf(m.tc_ext, 4, "dds");
    } else if (renderer_has_gl_ext("OES_compressed_ETC1_RGB8_texture")) {
        snprintf(m.tc_ext, 4, "ktx");
    } else {
        log_e("no texture compression extension supported on this device");
    }
}

int
renderer_init(sdl_window_t* window)
{
    log_d("initializing renderer");

    if (!window) {
        log_e("no window to initialize renderer, not sure what to do");
        return BINA_NOT_SURE;
    }

    m.window = window;

    /* m.renderer = sdl_create_renderer(window, -1, SDL_RENDERER_PRESENTVSYNC); */

    m.context = sdl_gl_create_context(window);

    if (!m.context) {
        log_c("unable to create gl context: %s", sdl_get_error());
        return BINA_LIB_ERROR;
    }

    /* vertical sync */
    sdl_gl_set_swap_interval(1);

    /* sdl_set_render_draw_color(m.renderer, 255, 0, 0, 0); */
    gl_clear_color(0.152f, 0.156f, 0.133f, 1.0f);

#ifdef HAVE_GL_GLEW_H
    bina_enum err = glew_init();

    if (err) {
        log_c("unable to initialize GLEW: %s", glew_get_error_string(err));
        return BINA_LIB_ERROR;
    }
#endif

    print_gl_string(GL_VERSION);
    print_gl_string(GL_VENDOR);
    print_gl_string(GL_RENDERER);
    print_gl_string(GL_EXTENSIONS);

    /* Getting OpenGL extensions. */
    m.gl_extensions = (char*) glGetString(GL_EXTENSIONS);

    /* Retrieve texture compression extension. */
    device_get_tc_ext();

    /* The following two lines enable semi transparent. */
    /* GL_CHECK(glEnable, GL_BLEND); */
    /* GL_CHECK(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */

    /* GL_CHECK(glDisable, GL_DEPTH_TEST); */
    /* GL_CHECK(glDepthMask, GL_FALSE); */

    return BINA_SUCCESS;
}

void
renderer_destroy()
{
    if (m.renderer) {
        sdl_destroy_renderer(m.renderer);
    }

    if (m.context) {
        sdl_gl_delete_context(m.context);
    }
}

void
renderer_render(void)
{
    float elapsed = sdl_get_ticks(); /* time since last frame */

    gl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* renderer_pre_render(0.0f, 0.4f, 1.0f, 1.0f); */
    /* scene_render(scene); */
    log_d("swaping window");
    sdl_gl_swap_window(m.window);
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

    if (strlen(m.gl_extensions)) {
        curs = strstr(m.gl_extensions, ext);
    }

    return (curs != NULL);
}

char*
renderer_get_tc_ext()
{
    return m.tc_ext;
}
