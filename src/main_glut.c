#include "bina.h"

void
AppDisplayCb(void) {
    glutSwapBuffers();
}
 
void
AppReshapeCb(int width, int height) {
    /*bina.SetViewport(0, 0, width, height);*/
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
    glutPostRedisplay();
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
    glutCreateWindow(APP_TITLE);

    /* Setting up callbacks */
    glutKeyboardFunc(AppNormalKeysCb);
    glutSpecialFunc(AppSpecialKeysCb);
    glutMouseFunc(AppMouseCb);
    glutReshapeFunc(AppReshapeCb);
    glutDisplayFunc(AppDisplayCb);
    glutIdleFunc(AppIdleCb);

    bina_init(GAME_WIDTH, GAME_HEIGHT);

    /*bina.Init(screenWidth, screenHeight);*/

    glutMainLoop();

    return 0;
}
