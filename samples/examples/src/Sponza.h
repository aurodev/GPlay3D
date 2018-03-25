#pragma once

#include "gplay3d.h"
#include "Example.h"
#include "FirstPersonCamera.h"

using namespace gameplay;


class Sponza : public Example
{
public:

    Sponza();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);


protected:


    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

    void updateCamera(float elapsedTime);


private:

    bool drawScene(Node* node);

private:

    Scene* _scene;

    Font* _font;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
    Model* _myModel;


    FirstPersonCamera _fpCamera;
    unsigned int _moveFlags;
    int _prevX;
    int _prevY;
    bool _buttonPressed;
};
