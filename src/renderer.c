/**
 * @file renderer.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

void
renderer_init()
{
    /* print_gl_string("Version", GL_VERSION); */
    /* print_gl_string("Vendor", GL_VENDOR); */
    /* print_gl_string("Renderer", GL_RENDERER); */
    /* print_gl_string("Extensions", GL_EXTENSIONS); */

    /* The following two lines enable semi transparent. */
    GL_CHECK(glEnable, GL_BLEND);
    GL_CHECK(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GL_CHECK(glDisable, GL_DEPTH_TEST);
    GL_CHECK(glDepthMask, GL_FALSE);
}

void
render_bina()
{
    float elapsed = main_get_time_elapsed();

    scene_animate(game.scene, elapsed);
    renderer_pre_render(0.0f, 0.4f, 1.0f, 1.0f);
    scene_render(game.scene);
}

void
renderer_pre_render(float r, float g, float b, float a)
{
    GL_CHECK(glClearColor, r, g, b, a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void
renderer_render()
{
    static float time = 0;
    static int   frames = 0;

    time += main_get_time_elapsed();
    frames++;

    if (time >= 1.0f) {
        /* LOGI("FPS: %d", frames); */
        time = 0.0f;
        frames = 0;
    }

    render_bina();
}
