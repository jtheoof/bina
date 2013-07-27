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
bina_init(int width, int height)
{
    vec2_t pos;
    pos.x = 0.0f;
    pos.y = 0.0f;

    viewport.x = 0;
    viewport.y = 0;
    viewport.width = width;
    viewport.height = height;

    background = sprite_create("../artwork/background.png", pos, 2.0f, 2.0f);
    brian = sprite_create("../artwork/brian.png", pos, 0.19f, 0.66f);

    /* No animator for now, only when gamer clicks or taps. */
    brian_animator = NULL;

    renderer_init(&viewport);
}

void
bina_end()
{

    sprite_delete(&background);
    sprite_delete(&brian);

    sprite_animator_delete(&brian_animator);
}

void
bina_animate_brian_to(vec2_t to, float speed)
{
    if (brian_animator) {
        sprite_animator_delete(&brian_animator);
    }

    brian_animator = sprite_animator_create(brian, to, speed);
}
