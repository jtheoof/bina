#include "bina.h"

game_background background;

void
bina_init(int width, int height)
{
    int error;

    background.file = "../artwork/background.png";

    glClearColor(0.0, 1.0, 1.0, 0.0);

    /* The following two lines enable semi transparent. */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    error = texture_load_png(background.file,
                             background.width, background.height,
                             background.alpha, &(background.image));
    if (error) {
        printf("unable to load image: %s\n", background.file);
        return;
    }

    /*SetOrthoProjection(width, height);*/
}
