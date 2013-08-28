/**
 * @file renderer.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"
#include "renderer.h"
/* TODO Perhaps remove dependency of scene.h */
#include "scene.h"

struct renderer_module_info {
    const char* gl_extensions;
};

static struct renderer_module_info m = {
    ""
};

void
renderer_init()
{
    /* print_gl_string("Version", GL_VERSION); */
    /* print_gl_string("Vendor", GL_VENDOR); */
    /* print_gl_string("Renderer", GL_RENDERER); */
    /* print_gl_string("Extensions", GL_EXTENSIONS); */

    /* Getting OpenGL extensions. */
    m.gl_extensions = (const char*) glGetString(GL_EXTENSIONS);

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
    const char* curs = strstr(m.gl_extensions, ext);

    return (curs != NULL);
}
