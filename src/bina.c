/**
 * @file bina.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

void
print_gl_string(const char* name, GLenum s)
{
    LOGI("GL %s = %s\n", name, glGetString(s));
}

void
bina_init(int width, int height)
{
    camera_viewport_t viewport;

    viewport.x = 0;
    viewport.y = 0;
    viewport.width = width;
    viewport.height = height;

    renderer_init(&viewport);

    scene = scene_load("bedroomArtist", 0.5f, 0.75f);
}

void
bina_end()
{
    scene_unload(&scene);
}

void bina_animate_porc_to(vec2_t to, float speed)
{
    scene_move_character_to(scene, to, speed);
}
