/**
 * @file scene.h
 * @author Jeremy Attali, Johan Attali
 * @date August 10, 2013
 */

#include "bina.h"

static void
load_scale_map(const char* name, const float minsize, const float maxsize,
               scene_t* scene)
{
    char       sm_buf[MAX_PATH];
    texture_t* sm_tex = NULL;

    snprintf(sm_buf, MAX_PATH, "scenes/%s/scaleMap.png", name);
    sm_tex = texture_create(sm_buf, 1);

    scene->smap       = sm_tex;
    scene->scale_min  = minsize;
    scene->scale_max  = maxsize;
    scene->scale_dif  = maxsize - minsize;

    /* If smap texture, upgrade the projection matrix */
    /* FIXME */
    if (0/*sm_tex*/) {
        /* scene->width  = sm_tex->width; */
        /* scene->height = sm_tex->height; */
    } else {
        scene->width  = 2.0f;
        scene->height = 2.0f;
    }

}

/* static void */
/* load_sprites_animations(const char* name, scene_t* scene) */
/* { */

/* } */

static void
load_sprites(const char* name, scene_t* scene)
{
    vec2_t pos, off, dim;

    /* Background */
    char       bg_buf[MAX_PATH];
    texture_t* bg_tex = NULL;
    sprite_t*  bg_spr = NULL;
    int        bg_prg = -1;

    /* Character */
    char       ch_buf[MAX_PATH];
    /* texture_t* ch_tex = NULL; */
    sprite_t*  ch_spr = NULL;
    int        ch_prg = -1;

    snprintf(bg_buf, MAX_PATH, "scenes/%s/scaleMap.png", name);
    snprintf(ch_buf, MAX_PATH, "animations/perso1_neutraPose_frontCam.png");

    bg_tex = texture_create(bg_buf, 0);
    /* ch_tex = texture_create(ch_buf, 0); */

    pos.x = pos.y = 0.0f;
    off.x = off.y = 0.0f;

    dim.x = scene->width;
    dim.y = scene->height;

    /* TODO Store these programes in a manager or in the scene. */
    bg_prg = shader_create_program(PROGRAM_BACKGROUND);
    ch_prg = shader_create_program(PROGRAM_CHARACTER);

    bg_spr = sprite_create(bg_tex, bg_prg, pos, off, dim, 1.0f);

    pos.x =  0.0f;
    pos.y = -1.0f;
    off.x =  0.0f;
    off.y =  0.5f;
    ch_spr = sprite_load_character("perso1", ch_prg, pos, off, dim, 1.0f);

    scene->background = bg_spr;
    scene->bg_prog    = bg_prg;
    scene->character  = ch_spr;
}

scene_t*
scene_load(const char* name, const float minsize, const float maxsize)
{
    scene_t* scene;
    mat4_t   projection;

    /* Updating projection matrix */
    float left, right, bottom, top;

    LOGI("loading scene: %s", name);

    scene = (scene_t*) malloc(sizeof(scene_t));
    if (!scene) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        return NULL;
    }

    load_scale_map(name, minsize, maxsize, scene);
    load_sprites(name, scene);

    if (scene->character) {
        scene->character->scale = maxsize;
    }

    left   = -2.0f / scene->width;
    right  =  2.0f / scene->width;
    bottom = -2.0f / scene->height;
    top    =  2.0f / scene->height;

    /* Initialize the projection matrix */
    projection = mat4_ortho(left, right, bottom, top, -1.0f, 1.0f);
    camera_set_projection(&projection);

    scene->time_idle = 0.0f;

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
            /* texture_delete(&tmp->character->texture); */
            sprite_delete(&tmp->character);
        }
        shader_delete_program(tmp->bg_prog);
        free(tmp);
        tmp = NULL;
    }

    *scene = tmp;
}

void
scene_animate(scene_t* scene, float elapsed)
{
    if (!scene || !scene->character) {
        return;
    }
    /* Compute scaling of sprite which depends on its position */
    /* size = scene_compute_character_size(scene, norm); */
    /* sprite_set_scale(scene->character, size); */
    scene->time_idle += elapsed;

    if (!scene->character->animator && scene->time_idle >= 2.0f) {
        sprite_animate_idle(scene->character);
    }

    /* If animation is over, delete the animator */
    if (sprite_animate(scene->character, elapsed) == SPRITE_ANIM_STATUS_DONE) {
        sprite_animator_delete(&scene->character->animator);
        scene->time_idle = 0.0f; /* reset timer */
    }
}

void
scene_render(scene_t* scene)
{
    static float angle = 0.0f;

    float  sin   = cosf(angle);
    /* mat4_t ident = mat4_identity(); */

    if (sin < 0) {
        sin *= -1.0f;
    }

    if (!scene) {
        return;
    }

    if (scene->background) {
        sprite_render(scene->background);
    }

    if (scene->character) {
        /* scene->character->mvp = mat4_scale_1f(&ident, sin); */
        sprite_render(scene->character);
    }

    angle += PI / 100.0f;
}

void
scene_move_character_to(scene_t* scene, vec2_t screen, float speed)
{
    float  size;
    vec2_t proj, norm;
    /* float  elapsed = main_get_time_elapsed(); */

    if (!scene || !scene->character) {
        return;
    }

    /* Convert screen point to model coordinate */
    proj = camera_screen_to_proj(&screen);

    /* Normalized Screen Coordinates point */
    norm = camera_normalize_screen_coord(&screen);

    /* Animate character to the given position */
    /* sprite_animate_char_to(scene->character, proj, speed, elapsed); */

    /* Compute scaling of sprite */
    size = scene_compute_character_size(scene, norm);
    sprite_set_scale(scene->character, size);

    LOGD("[character] previous position: %f, %f - new position: %f, %f",
         scene->character->position.x, scene->character->position.y,
         proj.x, proj.y);

    /* Update position of sprite */
    sprite_set_position(scene->character, &proj);
}

float
scene_compute_character_size(scene_t* scene, const vec2_t norm)
{
    unsigned int  width;  /* width of the scale map in pixels */
    unsigned int  height; /* height of the scale map in pixels */
    unsigned int  size;   /* how many bytes in the image */
    unsigned int  byte;   /* how many bytes per pixels */
    unsigned int  row;    /* row to look at */
    unsigned int  col;    /* column to look at */
    unsigned int  index;  /* actual index to study */

    unsigned char r, g, b, a = 0; /* final smap pixel color */

    float ret = 0.0f;

    if (!scene || !scene->smap || !scene->smap->pixels) {
        return 0.0f;
    }

    width  = scene->smap->width;
    height = scene->smap->height;
    size   = scene->smap->size;
    byte   = scene->smap->byte;

    /* XXX Because in OpenGL pixel rows are reversed (the first row in the
     * buffer is actually the last row of the image), we need to reverse the
     * order by pointing to the right row. Simply flip the normalized
     * y coordinates.
     */
    row = (unsigned int) ((1.0f - norm.y) * height);
    if (row >= height) {
        row = height - 1;
    }

    col = (unsigned int) (norm.x * width);
    if (col >= width) {
        col = width - 1;
    }

    /* From the normalized coordinates, it's easy to compute the data of the
     * pixels at this location. The pixels raw data stored in lines. Since we
     * know the width and height of the image and we also know how many bytes
     * there is for each pixel we can easily compute the location of the first
     * pixel to look for by using the following formula:
     * index = currentRow * rowBytes + currentColumn * byte;
     */
    index = row * byte * width + col * byte;

    if (byte < 3 || byte > 4) {
        LOGE("invalid byte size for scale map, should be RGB or RGBA");
        return 0.0f;
    }

    /* Finally getting the pixel color for the scale map.
     * Optionally, the scale map can also have an alpha, but it not used for
     * now.
     */
    r = scene->smap->pixels[index + 0];
    g = scene->smap->pixels[index + 1];
    b = scene->smap->pixels[index + 2];

    if (byte == 4) {
        a = scene->smap->pixels[index + 3];
    }

    ret = (r + g + b) / (3.0f * 255);

    LOGD("scale map pixel at: %d, %d (%d, %d, %d, %d)",
         (unsigned int) (norm.y * height), col, r, g, b, a);

    return scene->scale_min + (scene->scale_dif * ret);
}
