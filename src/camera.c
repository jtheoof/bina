#include "bina.h"

void
camera_set_viewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void
camera_set_ortho(int width, int height)
{
#if defined(USE_OPENGL_ES) || \
    defined(__ANDROID_API__)
#else
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
#endif
}
