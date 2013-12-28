/**
 * @file game.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"

void
game_init(game_t* game, int width, int height)
{
    camera_win_info_t viewport;

    /* Init randomizer. */
    srand(time(0));

    /* Set up the viewport. */
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = width;
    viewport.height = height;

    camera_set_viewport(&viewport);

    /* Load the scene. */
    game->scene = scene_load("bedroomArtist", 0.25f, 0.75f);
}

void
game_end(game_t* game)
{
    scene_unload(&game->scene);
}

void
game_render(scene_t* scene)
{
    renderer_render(scene);
}

void game_animate_porc_to(scene_t* scene, vec2_t to, float speed)
{
    scene_move_character_to(scene, to, speed);
}
