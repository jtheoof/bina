/**
 * @file renderer.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

/**
 * Elapsed time in ms since last frame.
 */
int elapsed_g = 0;
int current_g = 0;
int previous_g = 0;
int tmp = 0;

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
    float elapsed = renderer_get_time_elapsed();

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

float
renderer_get_time_elapsed()
{
    if (tmp++ >= 60) {
        /* LOGD("time elapsed: %f", current_g / 1000.0f); */
        LOGD("Previous: %d", previous_g);
        LOGD("Current: %d", current_g);
        LOGD("Elapsed: %f", elapsed_g / 1000.0f);
        tmp = 0;
    }
    return elapsed_g / 1000.0f;
}

void
renderer_render()
{
    /* TODO There might be a way to make this completely generic with
     * time.h */
#ifdef HAVE_GLUT_H
    /* LOGD("current_g: %i", current_g); */
    /* LOGD("elapsed_gl: %i", glutGet(GLUT_ELAPSED_TIME)); */
    previous_g = current_g;
    current_g = glutGet(GLUT_ELAPSED_TIME);
    elapsed_g = current_g - previous_g;
#else
    LOGE("You need to implemented elapsed time for this device");
    current_g = 16;
#endif
    render_bina();
}
