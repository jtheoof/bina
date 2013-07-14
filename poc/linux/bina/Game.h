#pragma once

#include <iostream>

#include "Bina.h"

#define GAME_WIDTH 940
#define GAME_HEIGHT 546

class Game
{
public:
    Game();
    void Init(int width, int height);
    void Render();
    int Loop();
    void SetViewport(int x, int y, int width, int height);
    void SetOrthoProjection(int width, int height);
    void MoveCharacter(float x, float y);

    void SetWindowWidth(int width) { _windowWidth = width; }
    void SetWindowHeight(int height) { _windowHeight = height; }
    int GetWindowWidth() { return _windowWidth; }
    int GetWindowHeight() { return _windowHeight; }

protected:
    GLubyte* _backgroundImage;
    int _backgroundWidth;
    int _backgroundHeight;
    bool _backgroundAlpha;

    GLubyte* _characterImage;
    int _characterWidth;
    int _characterHeight;
    bool _characterAlpha;

    float _characterX;
    float _characterY;
    
    int _windowWidth;
    int _windowHeight;

private:
    void RenderBackground();
    void RenderCharacter();
};

