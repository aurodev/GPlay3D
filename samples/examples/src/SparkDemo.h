#ifndef R_DYNAMIC_MESH_UPDATE_H_
#define R_DYNAMIC_MESH_UPDATE_H_

#include "gameplay.h"
#include "Example.h"

#include <spark/SPARK.h>
#include "GpSpark/SparkParticleEmitter.h"

using namespace gameplay;

class SparkDemo : public Example
{
public:

    SparkDemo();
    //~SparkDemo();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:

    bool drawScene(Node* node);

    Font* _font;
    Model* _triangleModel;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
    Model* _cubeModel;
    Node* _cubeNode;

    Node* _particleNode;

    Node* _cameraNode;
    Node* _cameraParent;

    Scene* _scene;
    SPK::Ref<SPK::System> __effectFountain;
    SparkParticleEmitter* myspksystem;
};

#endif
