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

int gWindowId;

void
AppDisplayCb(void) {
    renderer_render();
    glutSwapBuffers();
}
 
void
AppReshapeCb(int width, int height) {
}
 
void
AppNormalKeysCb(unsigned char key, int x, int y) {
}
 
void
AppSpecialKeysCb(int key, int x, int y) {
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

    glutDestroyWindow(gWindowId);
    /* glutPostRedisplay(); */
}

void
AppMouseCb(int button, int state, int x, int y) {
    /*int width = bina.GetWindowWidth(),
        height = bina.GetWindowHeight();*/

    /*std::cout << "Coordinates: (" << x << ", " << y << ")" << std::endl;*/

    /* Converting into bottom to top coordinates */
    /*bina.MoveCharacter(x, height - y);*/
    
    glutPostRedisplay();
}

void
AppIdleCb(void)
{
}

void
main_glut_exit(void)
{
    sprite_delete(&background);
}

int
main(int argc, char** argv)
{
    int screenWidth, screenHeight;
    int posX, posY;

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
    gWindowId = glutCreateWindow(APP_TITLE);
    
#ifdef HAVE_GLEW_H
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

    /* Setting up callbacks */
    glutKeyboardFunc(AppNormalKeysCb);
    glutSpecialFunc(AppSpecialKeysCb);
    glutMouseFunc(AppMouseCb);
    glutReshapeFunc(AppReshapeCb);
    glutDisplayFunc(AppDisplayCb);
    glutIdleFunc(AppIdleCb);

    bina_init(GAME_WIDTH, GAME_HEIGHT);

    atexit(main_glut_exit);

    glutMainLoop();

    return 0;
}
