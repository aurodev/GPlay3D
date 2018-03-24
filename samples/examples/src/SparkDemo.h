#pragma once

#include "gameplay.h"
#include "Example.h"
#include "FirstPersonCamera.h"
#include "sparkparticles/SparkParticleEmitter.h"
#include <spark/SPARK.h>

using namespace gameplay;

class SparkDemo : public Example
{
public:
    SparkDemo();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    void keyEvent(Keyboard::KeyEvent evt, int key);

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:
    bool updateEmitters(Node* node, float elapsedTime);
    bool drawScene(Node* node, int cookie);
    void showToolbox();

    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;
    Node* _cubeNode;
    Material* _materialParticle;
};
