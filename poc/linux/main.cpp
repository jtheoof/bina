#include "stdafx.h"

float rotateX = 0;
float rotateY = 0;

float translateX = 1.0;
float translateY = 0;
float translateZ = -1.0;

static int backgroundWidth = 0;
static int backgroundHeight = 0;
 
int mouseX;
int mouseY;

Game bina;
 
void
AppDisplayCb(void) {
    bina.Render();
    glutSwapBuffers();
}
 
void
AppReshapeCb(int width, int height) {
    //bina.SetOrthoProjection(width, height);
    bina.SetViewport(0, 0, width, height);
}
 
void
AppNormalKeysCb(unsigned char key, int x, int y) {
    std::cout << "Key: " << key << " has been pressed" << std::endl;
}
 
void
AppSpecialKeysCb(int key, int x, int y) {
    std::cout << "Key: " << key << " has been pressed" << std::endl;
    float speed = 5.0;
    switch (key) {
        case GLUT_KEY_F1:
            std::cout << "F1" << std::endl;
            break;
        case GLUT_KEY_UP:
            translateZ += speed;
            break;
        case GLUT_KEY_DOWN:
            translateZ -= speed;
            break;
        case GLUT_KEY_LEFT:
            translateX += speed;
            break;
        case GLUT_KEY_RIGHT:
            translateX -= speed;
            break;
        case GLUT_KEY_PAGE_UP:
            translateY += speed;
            break;
        case GLUT_KEY_PAGE_DOWN:
            translateY -= speed;
            break;
    }
    std::cout << "Position: ("
              << translateX << ", "
              << translateY << ", "
              << translateZ << ")" << std::endl;
    glutPostRedisplay();
}

void
AppMouseCb(int button, int state, int x, int y) {
    int width = bina.GetWindowWidth(),
        height = bina.GetWindowHeight();

    std::cout << "Coordinates: (" << x << ", " << y << ")" << std::endl;

    /* Converting into bottom to top coordinates */
    bina.MoveCharacter(x, height - y);
    
    glutPostRedisplay();
}

void
AppIdleCb(void)
{
}
 
int
main(int argc, char** argv)
{
	int screenWidth, screenHeight,
        posX, posY;

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

    bina.Init(screenWidth, screenHeight);

    glutMainLoop();

    return 0;
}
