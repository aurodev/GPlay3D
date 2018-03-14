#include "SparkParticleEmitter.h"
#include "Scene.h"
#include "SparkBaseRenderer.h"

namespace gameplay {


SparkParticleEmitter::SparkParticleEmitter() : Drawable()
{
    _worldTransformed = false;
    _onlyWhenVisible = false;
    _alive = true;
}

SparkParticleEmitter::~SparkParticleEmitter()
{

}

SparkParticleEmitter* SparkParticleEmitter::create(SPK::Ref<SPK::System> sparkSystem, bool worldTransformed)
{
    SparkParticleEmitter* sparkParticleEmitter = new SparkParticleEmitter();
    sparkParticleEmitter->_sparkSystem = SPK::SPKObject::copy(sparkSystem);
    sparkParticleEmitter->_worldTransformed = worldTransformed;
    return sparkParticleEmitter;
}

Drawable* SparkParticleEmitter::clone(NodeCloneContext& context)
{

}

void SparkParticleEmitter::updateCameraPosition() const
{
    Camera* camera = _node->getScene()->getActiveCamera();

    Matrix invTransform;
    invTransform = camera->getInverseViewMatrix();

    SPK::GP3D::SparkBaseRenderer* renderer = nullptr;

    for (size_t i=0; i<_sparkSystem->getNbGroups(); ++i)
    {
        // set view matrix for renderer
        renderer = reinterpret_cast<SPK::GP3D::SparkBaseRenderer*>(_sparkSystem->getGroup(i)->getRenderer().get());
        GP_ASSERT(renderer);
        renderer->setViewMatrix(invTransform);

        // set spark camera position when group do sorting or distance computation
        if (_sparkSystem->getGroup(i)->isDistanceComputationEnabled())
        {
            Vector3 pos = camera->getNode()->getTranslationWorld();
            _sparkSystem->setCameraPosition(SPK::GP3D::gp3d2spk(pos));
        }
    }
}


void SparkParticleEmitter::update(float dt)
{
    if(_node && _node->isEnabled() && !_onlyWhenVisible)
    {
        if (_worldTransformed)
        {
            // move spark system using node transformation
            _sparkSystem->getTransform().set(_node->getWorldMatrix().m);
        }

        updateCameraPosition();

        _alive = _sparkSystem->updateParticles(dt * 0.001f);
    }
}

unsigned int SparkParticleEmitter::draw(bool wireframe)
{
    if(_alive)
    {
        _sparkSystem->renderParticles();
        return 1;
    }

    return 0;
}

}
