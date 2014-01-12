/**
 * @file bina.c
 * @author Jeremy Attali
 */

#include "bina.h"
#include "log.h"
#include "renderer.h"

/**
 * Module structure.
 */
struct bina_module_info {
    sdl_window_t* window;
};

/**
 * Module static object.
 */
static struct bina_module_info m = {
    NULL
};

int
bina_init(bina_window_t* window)
{
    int err;

    /* Init SDL. */
    if (sdl_init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "sdl: unable to initialize: %s\n", sdl_get_error());
        return BINA_LIB_ERROR;
    }

    /* Be verbose in logs. */
    sdl_log_set_all_priority(SDL_LOG_PRIORITY_VERBOSE);

#ifdef HAVE_STDIO_H
    /* Set up our own custom log function without userdata. */
    sdl_log_set_output_function(log_custom, NULL);
#endif

    if (window) {
        m.window = sdl_create_window(window->title, SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     window->width, window->height,
                                     SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (!m.window) {
            sdl_log_error(SDL_LOG_CATEGORY_APPLICATION,
                          "sdl: unable to create window: %s", sdl_get_error());
            return BINA_LIB_ERROR;
        }
    }

    return renderer_init(m.window);
}

int
bina_render()
{
    renderer_render(NULL);

    return BINA_SUCCESS;
}

void
bina_exit()
{
    renderer_destroy();

    if (m.window) {
        sdl_destroy_window(m.window);
    }

    log_d("exiting library");
    sdl_quit();
}
