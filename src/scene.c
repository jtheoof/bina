/**
 * @file scene.h
 * @author Jeremy Attali, Johan Attali
 * @date August 10, 2013
 */

#include "bina.h"
#include "scene.h"
#include "shader.h"
#include "camera.h"

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
    sprite_t*  ch_spr = NULL;
    int        ch_prg = -1;

    snprintf(bg_buf, MAX_PATH, "scenes/%s/backgroundColor.png", name);
    snprintf(ch_buf, MAX_PATH, "animations/perso1_neutraPose_frontCam.png");

    bg_tex = texture_create(bg_buf, 0);

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
    scene->ch_prog    = ch_prg;
    scene->character  = ch_spr;
}

static void
load_dds(scene_t* scene)
{
    vec2_t pos, off, dim;
    texture_t* dds_tex;
    sprite_t*  dds_spr;
    int        dds_prg;

    pos.x = pos.y = 0.0f;
    off.x = off.y = 0.0f;

    scene->width  = 2.0f;
    scene->height = 2.0f;

    dim.x = scene->width;
    dim.y = scene->height;

    dds_tex = texture_create("scenes/testDDS/dxt1.rgb.dds", 0);
    /* dds_tex = texture_create("scenes/testDDS/earth.dds", 0); */
    dds_prg = shader_create_program(PROGRAM_BACKGROUND);
    dds_spr = sprite_create(dds_tex, dds_prg, pos, off, dim, 1.0f);

    scene->background = dds_spr;
    scene->bg_prog    = dds_prg;
    scene->ch_prog    = 0;
    scene->character  = NULL;
    scene->smap       = NULL;
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

    scene->is_ready = 0;

    if (0) {
        load_scale_map(name, minsize, maxsize, scene);
        load_sprites(name, scene);
    }

    load_dds(scene);

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
    scene->is_ready  = 1;

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
        shader_delete_program(tmp->ch_prog);
        free(tmp);
        tmp = NULL;
    }

    *scene = tmp;
}

void
scene_animate(scene_t* scene, float elapsed)
{
    sprite_t* character;
    sprite_animator_t* animator;
    sprite_tex_anim_t* texanims;
    sprite_anim_status_e status;
    vec2_t ndc;
    float  size;

    if (!scene || !scene->character) {
        return;
    }

    scene->time_idle += elapsed;
    character = scene->character;
    animator  = character->animator;
    texanims  = character->tex_anims;

    /* Compute scaling of sprite which depends on its position */
    ndc = camera_eye_coord_to_ndc(character->position);
    size = scene_compute_character_size(scene, ndc);
    sprite_set_scale(character, size);

    status = sprite_animate(character, elapsed);

    /* If animation is over, delete the animator */
    if (status == SPRITE_ANIM_STATUS_DONE) {
        sprite_animator_delete(&character->animator);

        if (texanims && texanims->cur_anim == SPRITE_ANIM_STOPACTION1) {
            scene->time_idle = 0.0f; /* reset timer */
        }
    }

    if (!animator) {
        if (scene->time_idle >= 2.0f) {
            sprite_animate_idle(character, SPRITE_ANIM_STOPACTION1);
        } else {
            sprite_animate_idle(character, SPRITE_ANIM_NEUTRALPOSE);
        }
    }

}

void
scene_render(scene_t* scene)
{
    mat4_t proj = camera_get_projection();

    if (!scene) {
        return;
    }

    if (scene->background) {
        sprite_compute_mvp(scene->background, &proj);
        sprite_render(scene->background);
    }

    if (scene->character) {
        sprite_compute_mvp(scene->character, &proj);
        sprite_render(scene->character);
    }
}

void
scene_move_character_to(scene_t* scene, vec2_t screen, float speed)
{
    float elapsed = main_get_time_elapsed(), alpha;
    vec2_t proj, norm;

    if (!scene || !scene->character) {
        return;
    }

    /* Convert screen point to model coordinate */
    proj = camera_win_coord_to_eye(&screen);

    /* Normalized Screen Coordinates point */
    norm = camera_win_coord_to_ndc(&screen);

    LOGD("[character] previous position: %f, %f - new position: %f, %f",
         scene->character->position.x, scene->character->position.y,
         proj.x, proj.y);

    /* TODO Do this in a more clever way (see todo in next function). */
    alpha = scene_compute_character_size(scene, norm);

    if (alpha > 0) {
        /* Update position of sprite */
        sprite_animate_char_to(scene->character, proj, 1.0f, elapsed);
    }
}

/* TODO Refactor this function so that it does not return 0 if alpha color is
 * found in the end pixel. Right now this feature is used as a way to prevent
 * the user to click on that area. This should be refactored.
 */
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

    unsigned char r, g, b, a = 255; /* final smap pixel color */

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

        if (a == 0) {
            return 0.0f;
        }
    }

    ret = (r + g + b) / (3.0f * 255);

    /* LOGD("scale map pixel at: %d, %d (%d, %d, %d, %d)", */
    /*      (unsigned int) (norm.y * height), col, r, g, b, a); */

    return scene->scale_min + (scene->scale_dif * ret);
}
