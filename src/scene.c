/**
 * @file scene.h
 * @author Jeremy Attali, Johan Attali
 * @date August 10, 2013
 */

#include "bina.h"

scene_t*
scene_load(const char* name, const float minsize, const float maxsize)
{
    scene_t* scene;

    vec2_t pos;
    vec2_t off;
    vec2_t siz;


    /* Background */
    char       bg_buf[MAX_PATH];
    texture_t* bg_tex = NULL;
    sprite_t*  bg_spr = NULL;
    int        bg_prg = -1;

    /* Scale map */
    char       sm_buf[MAX_PATH];
    texture_t* sm_tex = NULL;

    /* Character */
    char       ch_buf[MAX_PATH];
    texture_t* ch_tex = NULL;
    sprite_t*  ch_spr = NULL;
    int        ch_prg = -1;

    LOGI("loading scene: %s", name);

    scene = (scene_t*) malloc(sizeof(scene_t));
    if (!scene) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        return NULL;
    }

    snprintf(bg_buf, MAX_PATH, "scenes/%s/background.png", name);
    snprintf(sm_buf, MAX_PATH, "scenes/%s/scaleMap.png", name);
    bg_tex = texture_create(bg_buf, 0);
    sm_tex = texture_create(sm_buf, 1);

    snprintf(ch_buf, MAX_PATH, "animations/perso1_neutraPose_frontCam.png");
    ch_tex = texture_create(ch_buf, 0);

    pos.x = pos.y = 0.0f;
    off.x = off.y = 0.0f;
    siz.x = siz.y = 2.0f;

    bg_prg = shader_create_program(PROGRAM_BACKGROUND);
    bg_spr = sprite_create(bg_tex, bg_prg, pos, off, siz);

    ch_prg = shader_create_program(PROGRAM_BACKGROUND);
    ch_spr = sprite_create(ch_tex, ch_prg, pos, off, siz);

    scene->background = bg_spr;
    scene->smap       = sm_tex;
    scene->minsize    = minsize;
    scene->maxsize    = maxsize;
    scene->bg_prog    = bg_prg;
    scene->character  = ch_spr;

    return scene;
}

void
scene_unload(scene_t** scene)
{
    scene_t* tmp = *scene;

    if (tmp) {
        if (tmp->background) {
            texture_delete(&tmp->background->texture);
            sprite_delete(&tmp->background);
        }
        if (tmp->smap) {
            texture_delete(&tmp->smap);
        }
        if (tmp->character) {
            texture_delete(&tmp->character->texture);
            sprite_delete(&tmp->character);
        }
        shader_delete_program(tmp->bg_prog);
        free(tmp);
        tmp = NULL;
    }

    *scene = tmp;
}

void
scene_render(scene_t* scene)
{
    if (!scene) {
        return;
    }

    if (scene->background) {
        sprite_render(scene->background);
    }

    if (scene->character) {
        sprite_render(scene->character);
    }
}
