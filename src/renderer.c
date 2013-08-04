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

/* void */
/* animate_sprites_demo(float elapsed) */
/* { */
/*     unsigned int i, s; */

/*     for (i = 0; i < SPRITES_DEMO; i++) { */
/*         /1* if (animators_demo[i]) { *1/ */
/*         /1*     s = sprite_animator_animate(sprites_demo[i], animators_demo[i], *1/ */
/*         /1*                                 elapsed); *1/ */
/*         /1*     if (!s) { *1/ */
/*         /1*         LOGD("Sprite: %d finished", i); *1/ */
/*         /1*         sprite_animator_delete(&animators_demo[i]); *1/ */
/*         /1*         bina_animate_demo_sprite(sprites_demo[i], &animators_demo[i], *1/ */
/*         /1*                                  elapsed); *1/ */
/*         /1*     } *1/ */
/*         /1* } *1/ */
/*         if (sprites_demo[i]) { */
/*             sprite_render(sprites_demo[i]); */
/*         } */
/*     } */
/* } */

static void
update_porc_animation(float elapsed)
{
    static float time = 0.0f;
    static int id = 0;

    time += elapsed;

    /* Increase texture id for Real time rendering at 24 fps */
    if (time >= 0.042f) {
        id++;
        if (id >= porc_r_tex->size) {
            id = 0;
        }
        time = 0.0f;
    }

    if (ani_porc) {
        if (!sprite_animator_animate(porc, ani_porc, elapsed)) {
            sprite_animator_delete(&ani_porc);
        }
    }

    /* sprite_set_texture(porc, porc_r_tex->textures[id]); */
}

void
render_bina()
{
    /* main_get_time_elapsed is defined in main_[platform].c */
    float elapsed = main_get_time_elapsed();

    renderer_pre_render(0.0f, 0.4f, 1.0f, 1.0f);

    update_porc_animation(elapsed);

    sprite_render(back);
    sprite_render(porc);
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
        LOGI("FPS: %d", frames);
        time = 0.0f;
        frames = 0;
    }

    render_bina();
}
