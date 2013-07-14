#include "Game.h"

Game::Game()
{
    _characterX = 0.0;
    _characterY = 0.0;
}

void
Game::Init(int width, int height) {
    char background[] = "bina.png";
    char character[] = "brian.png";
    bool success;

    glClearColor(0.0, 0.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    // The following two lines enable semi transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    success = loadPngImage(background, _backgroundWidth,
                                _backgroundHeight, _backgroundAlpha,
                                &_backgroundImage);
    if (!success) {
        std::cout << "Unable to load png file" << std::endl;
        return;
    }

    success = loadPngImage(character, _characterWidth,
                           _characterHeight, _characterAlpha,
                           &_characterImage);
    if (!success) {
        std::cout << "Unable to load png file" << std::endl;
        return;
    }


    std::cout << "background: (" << _backgroundWidth << "x"
              << _backgroundHeight << ")" << std::endl;
    std::cout << "character: (" << _characterWidth << "x"
              << _characterHeight << ")" << std::endl;
    std::cout << "backgroundAlpha: " << _backgroundAlpha << std::endl;
    std::cout << "characterAlpha: " << _characterAlpha << std::endl;

    SetOrthoProjection(width, height);
}

void
Game::MoveCharacter(float x, float y)
{
    _characterX = x;
    _characterY = y;
}

void
Game::RenderBackground()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, _backgroundAlpha ? 4 : 3, _backgroundWidth,
                 _backgroundHeight, 0, _backgroundAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                 _backgroundImage);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_POLYGON);

        glTexCoord2f(0.0, 0.0);
        glVertex2f(0, 0);

        glTexCoord2f(0.0, 1.0);
        glVertex2f(0, _windowHeight);

        glTexCoord2f(1.0, 1.0);
        glVertex2f(_windowWidth, _windowHeight);

        glTexCoord2f(1.0, 0.0);
        glVertex2f(_windowWidth, 0);
 
    glEnd();
}

void
Game::RenderCharacter()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, _characterWidth,
                 _characterHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 _characterImage);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_POLYGON);

        glTexCoord2f(0.0, 0.0);
        glVertex2f(_characterX, _characterY);

        glTexCoord2f(0.0, 1.0);
        glVertex2f(_characterX, _characterY + _characterHeight);

        glTexCoord2f(1.0, 1.0);
        glVertex2f(_characterX + _characterWidth,
                   _characterY + _characterHeight);

        glTexCoord2f(1.0, 0.0);
        glVertex2f(_characterX + _characterWidth, _characterY);
 
    glEnd();
}

void
Game::Render()
{
    SetOrthoProjection(_windowWidth, _windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.375, 0.375, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderCharacter();
    RenderBackground();
}

void
Game::SetViewport(int x, int y, int width, int height)
{
    _windowWidth = width;
    _windowHeight = height;

    glViewport(x, y, width, height);
}

void
Game::SetOrthoProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1, 1, -1, 1, -1, 1);
    glOrtho(0, _windowWidth, 0, _windowHeight, -1, 1);
}
