#ifndef R_DYNAMIC_MESH_UPDATE_H_
#define R_DYNAMIC_MESH_UPDATE_H_

#include "gameplay.h"
#include "Example.h"
#include "FirstPersonCamera.h"
#include "ParticleSpark/SparkParticleEmitter.h"
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
    bool drawScene(Node* node);
    void showToolbox();

    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;
    Node* _cubeNode;
    Material* _materialParticle;
};

#endif
