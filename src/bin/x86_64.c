#include "bina.h"

int
main(int argc, char** argv)
{
    bina_window_t window = {
        .title = "Bina",
        .width = 800,
        .height = 600
    };
    int err;

    err = bina_init(&window);

    if (err) {
        exit(-1);
    }

    bina_render();

    bina_exit();

    return 0;
}
