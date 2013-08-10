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
    bina_load_background("scenes/bedroomArtist/background.png");
    /* bina_load_porc(); */

    viewport.x = 0;
    viewport.y = 0;
    viewport.width = width;
    viewport.height = height;
    renderer_init(&viewport);
}

void
bina_end()
{
    /* texture_delete(&back_tex); */
    /* texture_delete_list(&porc_r_tex); */
    /* texture_delete_list(&porc_l_tex); */

    /* sprite_delete(&back); */
    /* sprite_delete(&porc); */

    /* sprite_animator_delete(&ani_porc); */
    scene_unload(&scene);
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

    /* back_tex = texture_create(filepath); */
    /* back = sprite_create(back_tex, pos, off, size); */
    scene = scene_load("bedroomArtist", 0.5f, 0.75f);
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
    texture_list_t* list = NULL;

    if (ani_porc) {
        sprite_animator_delete(&ani_porc);
    }

    if (porc->position.x <= to.x) {
        list = porc_r_tex;
    } else {
        list = porc_l_tex;
    }

    ani_porc = sprite_animator_create(porc, list, to, speed, elapsed);
}
