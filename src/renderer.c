/**
 * @file renderer.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

void
renderer_init(camera_viewport_t* viewport)
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

    /* Set up the viewport */
    camera_set_viewport(viewport);
}

static void
render_bina()
{
    /* main_get_time_elapsed is defined in main_[platform].c */
    float elapsed = main_get_time_elapsed();

    renderer_pre_render(0.0f, 0.4f, 1.0f, 1.0f);

    if (brian_animator) {
        if (!sprite_animator_animate(brian, brian_animator, elapsed)) {
            sprite_animator_delete(&brian_animator);
        }
    }

    sprite_render(background);
    sprite_render(brian);
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
    static int tmp = 0;

    if (tmp++ >= 60) {
        tmp = 0;
        LOGD("Time elapsed: %f", main_get_time_elapsed());
    }

    render_bina();
}
