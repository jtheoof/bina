/**
 * Main GLUT file used for oses that support GLUT.
 *
 * It should be possible to write a very simple wrapper around GLUT for
 * devices that do not support it.
 *
 * @see http://freeglut.sourceforge.net/docs/android.php
 * @see http://en.wikibooks.org/wiki/OpenGL_Programming/Android_GLUT_Wrapper
 *
 * (GLUT does not seem to have been ported to iOS).
 */

/**
 * @file main_glut.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

static int window_id_g = 0;

/** Number of miliseconds since last frame was drawn. */
static int elap_time_g = 0;

float
main_get_time_elapsed()
{
    return elap_time_g / 1000.0f;
}

void
main_glut_display_cb(void)
{
    static int prev_time = 0;
    static int curr_time = 0;

    curr_time = glutGet(GLUT_ELAPSED_TIME);
    
    elap_time_g = curr_time - prev_time;
    prev_time = curr_time;
    
    renderer_render();
    glutSwapBuffers();
}

void
main_glut_timer_cb(int t)
{
    glutPostRedisplay();
    
    glutTimerFunc(GAME_REFRESH_RATE, main_glut_timer_cb, 0);
}

void
main_glut_reshape_cb(int width, int height)
{
    viewport.width = width;
    viewport.height = height;

    /* Reset the viewport */
    camera_set_viewport(&viewport);
}

void
main_glut_normal_keys_cb(unsigned char key, int x, int y)
{
    switch (key) {
        case 27: /* Escape key */
            glutDestroyWindow(window_id_g);
            break;
    }
}

void
main_glut_special_keys_cb(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            break;
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
        case GLUT_KEY_PAGE_UP:
            break;
        case GLUT_KEY_PAGE_DOWN:
            break;
    }

    glutDestroyWindow(window_id_g);
}

void
main_glut_mouse_cb(int button, int state, int x, int y)
{
    vec2_t coord;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        coord.x = 2.0f * x / viewport.width;
        coord.y = 2.0f - (2.0f * y / viewport.height);

        LOGD("Screen: %d,%d", x, y);
        LOGD("Viewport: %f,%f", coord.x, coord.y);

        bina_animate_porc_to(coord, 2.0f);
    }

    glutPostRedisplay();
}

void
main_glut_idle_cb(void)
{
}

void
main_glut_exit(void)
{
    bina_end();
}

int
main(int argc, char** argv)
{
    int screenWidth, screenHeight;
    int posX, posY;
	GLenum err;

    /* Init GLUT */
    glutInit(&argc, argv);

	/* RGBA with double buffer */
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	/* Create centered window */
	screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	posX = (screenWidth >> 1) - (GAME_WIDTH >> 1);
	posY = (screenHeight >> 1) - (GAME_HEIGHT >> 1);

	glutInitWindowPosition(posX, posY);
	glutInitWindowSize(GAME_WIDTH, GAME_HEIGHT);
    window_id_g = glutCreateWindow(APP_TITLE);

#ifdef HAVE_GL_GLEW_H
    err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        LOGE("Could not initialize glew");
        exit(-1);
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

    /* Setting up callbacks */
    glutKeyboardFunc(main_glut_normal_keys_cb);
    glutSpecialFunc(main_glut_special_keys_cb);
    glutMouseFunc(main_glut_mouse_cb);
    glutReshapeFunc(main_glut_reshape_cb);
    glutDisplayFunc(main_glut_display_cb);
    glutTimerFunc(GAME_REFRESH_RATE, main_glut_timer_cb, 0);

    bina_init(GAME_WIDTH, GAME_HEIGHT);

    atexit(main_glut_exit);

    glutMainLoop();

    return 0;
}
