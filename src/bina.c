/**
 * @file bina.c
 * @author Jeremy Attali
 */

#include "bina.h"
#include "renderer.h"

int
bina_init()
{
    if (sdl_init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr,
                "unable to initialize SDL:  %s\n",
                sdl_get_error());
        return 1;
    }
}

int
bina_exit()
{
    sdl_log_info(SDL_LOG_CATEGORY_APPLICATION, "exiting library");
    sdl_quit();
}
