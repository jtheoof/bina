/**
 * @file bina.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

static const GLubyte*
gl_error_string(GLenum errorCode)
{
    int i;
    for (i = 0; gl_errors[i].string; i++) {
        if (gl_errors[i].token == errorCode)
            return (const GLubyte *) gl_errors[i].string;
    }

    return (const GLubyte *) 0;
}

void
print_gl_string(const char* name, GLenum s)
{
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void
check_gl_error(void)
{
    GLint e;
    for (e = glGetError(); e; e = glGetError()) {
        LOGE("glError: %s (0x%x)\n", gl_error_string(e), e);
    }
}

void
bina_animate_demo_sprite(sprite_t* sprite, sprite_animator_t** animator,
                         float elapsed)
{
    long int rx, ry;
    vec2_t rto;

    rx = rand() % 2001;
    ry = rand() % 2001;

    rto.x = (rx / 2000.0f * 2.0f) - 0.0f;
    rto.y = (ry / 2000.0f * 2.0f) - 0.0f;

    /* *animator = sprite_animator_create(sprite, rto, 1.0f, elapsed); */
}

void
bina_init(int width, int height)
{
    bina_load_background("backgrounds/bedroom_artist.png");
    bina_load_porc();

    viewport.x = 0;
    viewport.y = 0;
    viewport.width = width;
    viewport.height = height;
    renderer_init(&viewport);
}

void
bina_end()
{
    /* int i; */

    /* if (sprites_demo) { */
    /*     for (i = 0; i < SPRITES_DEMO; i++) { */
    /*         sprite_delete(&sprites_demo[i]); */
    /*     } */
    /*     free(sprites_demo); */
    /* } */

    /* if (animators_demo) { */
    /*     for (i = 0; i < SPRITES_DEMO; i++) { */
    /*         sprite_animator_delete(&animators_demo[i]); */
    /*     } */
    /*     free(animators_demo); */
    /* } */

    texture_delete(&back_tex);
    texture_delete_list(&porc_r_tex);
    texture_delete_list(&porc_l_tex);

    sprite_delete(&back);
    sprite_delete(&porc);

    sprite_animator_delete(&ani_porc);
}

void
bina_load_background(const char* filepath)
{
    vec2_t pos, off, size;

    pos.x = 0.0f;
    pos.y = 0.0f;

    off.x = 0.0f;
    off.y = 0.0f;

    size.x = 2.0f;
    size.y = 2.0f;

    back_tex = texture_create(filepath);
    back = sprite_create(back_tex, pos, off, size);
}

void
bina_load_porc()
{
    vec2_t pos, off, size;

    pos.x = 0.5f;
    pos.y = 0.2f;

    off.x = 0.43f;
    off.y = 0.2f;

    size.x = 0.86f;
    size.y = 1.53f;

    porc_r_tex = texture_create_list("perso1_walkCycle_cameraLeft", "png", 21);
    porc_l_tex = texture_create_list("perso1_walkCycle_cameraRight", "png", 21);
    porc = sprite_create(porc_r_tex->textures[0], pos, off, size);

    /* No animator for now, only when gamer clicks or taps. */
    ani_porc = NULL;
}

void
bina_animate_porc_to(vec2_t to, float speed)
{
    float elapsed = main_get_time_elapsed();

    if (ani_porc) {
        sprite_animator_delete(&ani_porc);
    }

    ani_porc = sprite_animator_create(porc, NULL, to, speed, elapsed);
}
