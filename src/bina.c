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

    *animator = sprite_animator_create(sprite, rto, 1.0f, elapsed);
}

void
bina_init(int width, int height)
{
    int i;
    long int rx, ry;
    float elapsed = main_get_time_elapsed();

    vec2_t pos;
    vec2_t rpos;

    pos.x = 0.0f;
    pos.y = 0.0f;

    viewport.x = 0;
    viewport.y = 0;
    viewport.width = width;
    viewport.height = height;

    background = sprite_create("artwork/background.png", pos, 2.0f, 2.0f);
    brian = sprite_create("artwork/brian.png", pos, 0.19f, 0.66f);

    sprites_demo = (sprite_t**) calloc(SPRITES_DEMO, sizeof(sprite_t*));
    animators_demo = (sprite_animator_t**) calloc(SPRITES_DEMO,
                                                  sizeof(sprite_animator_t*));

    for (i = 0; i < SPRITES_DEMO; i++) {
        rx = rand() % 2001;
        ry = rand() % 2001;

        rpos.x = (rx / 2000.0f * 2.0f) - 0.0f;
        rpos.y = (ry / 2000.0f * 2.0f) - 0.0f;

        sprites_demo[i] = sprite_create("artwork/brian.png", rpos,
                                        0.19f, 0.66f);

        bina_animate_demo_sprite(sprites_demo[i], &animators_demo[i], 0.016f);
    }

    /* No animator for now, only when gamer clicks or taps. */
    brian_animator = NULL;

    renderer_init(&viewport);
}

void
bina_end()
{
    int i;

    if (sprites_demo) {
        for (i = 0; i < SPRITES_DEMO; i++) {
            sprite_delete(&sprites_demo[i]);
        }
        free(sprites_demo);
    }

    if (animators_demo) {
        for (i = 0; i < SPRITES_DEMO; i++) {
            sprite_animator_delete(&animators_demo[i]);
        }
        free(animators_demo);
    }

    sprite_delete(&background);
    sprite_delete(&brian);

    sprite_animator_delete(&brian_animator);
}

void
bina_animate_brian_to(vec2_t to, float speed)
{
    float elapsed = main_get_time_elapsed();

    if (brian_animator) {
        sprite_animator_delete(&brian_animator);
    }

    brian_animator = sprite_animator_create(brian, to, speed, elapsed);
}
