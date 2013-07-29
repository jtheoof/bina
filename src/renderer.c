/**
 * @file renderer.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

/**
 * Elapsed time in ms since last frame.
 */
int64_t elapsed_g  = 0;
int64_t current_g  = 0;
int64_t previous_g = 0;

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

/* TODO Move specific code to approriate file */
int64_t
renderer_get_time_nano()
{
#ifdef UNIX
    struct timespec t;

    t.tv_sec = t.tv_nsec = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &t)) {
        LOGE("clock_gettime returned an error");
        return 0;
    }

    return (int64_t)(t.tv_sec)*1000000000LL + t.tv_nsec;
#else
    LOGE(BINA_NOT_IMPLEMENTED);

    return 0;
#endif
}

float
renderer_get_time_elapsed()
{
    return elapsed_g / 1000000000.0f;
}

void
renderer_render()
{
    static int tmp = 0;
    /* TODO There might be a way to make this completely generic with
     * time.h */

    /* LOGD("current_g: %i", current_g); */
    /* LOGD("elapsed_gl: %i", glutGet(GLUT_ELAPSED_TIME)); */
    if (tmp++ >= 100) {
        tmp = 0;
        LOGD("Current time: %ld", renderer_get_time_nano());
        LOGD("Time elapsed: %f",  renderer_get_time_elapsed());
    }
    previous_g = current_g;
    current_g = (int64_t) renderer_get_time_nano();
    elapsed_g = current_g - previous_g;

    render_bina();
}
