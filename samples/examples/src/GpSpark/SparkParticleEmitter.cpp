#include "SparkParticleEmitter.h"
#include "Scene.h"
#include "SparkBaseRenderer.h"

namespace gameplay {


SparkParticleEmitter::SparkParticleEmitter() : Drawable()
{

}

SparkParticleEmitter::~SparkParticleEmitter()
{

}

SparkParticleEmitter* SparkParticleEmitter::create(SPK::Ref<SPK::System> sparkSystem)
{
    SparkParticleEmitter* sparkParticleEmitter = new SparkParticleEmitter();
    sparkParticleEmitter->_sparkSystem = SPK::SPKObject::copy(sparkSystem);
    return sparkParticleEmitter;
}

Drawable* SparkParticleEmitter::clone(NodeCloneContext& context)
{

}

void SparkParticleEmitter::update(float dt)
{
    //GP_ASSERT(_node && _node->getScene() && _node->getScene()->getActiveCamera() && _node->getScene()->getActiveCamera()->getNode());
    //const Matrix& cameraWorldMatrix = _node->getScene()->getActiveCamera()->getNode()->getWorldMatrix();

    gameplay::Matrix invModelView;
    //invModelView.transpose();
    //invModelView.invert();

    if(_node)
    {
        Node* cameraNode = _node->getScene()->getActiveCamera()->getNode();
        Camera* camera = cameraNode->getCamera();

        //invModelView = camera->getViewProjectionMatrix();// getViewMatrix();
        //invModelView.transpose();
        //invModelView.invert();

        invModelView = _node->getWorldViewProjectionMatrix();//camera->getInverseViewMatrix();
        //invModelView.transpose();
        invModelView.invert();

        SPK::Vector3D vpos(cameraNode->getTranslation().x, cameraNode->getTranslation().y, cameraNode->getTranslation().z);
        _sparkSystem->setCameraPosition(vpos);

        //_sparkSystem->getTransform().setNC(_node->getMatrix().m);
    }


    for(size_t i=0; i<_sparkSystem->getNbGroups(); ++i)
    {
        SPK::GP3D::SparkBaseRenderer* renderer =
                reinterpret_cast<SPK::GP3D::SparkBaseRenderer*>(_sparkSystem->getGroup(i)->getRenderer().get());

        if(renderer)
        {
            renderer->setViewMatrix(invModelView);
        }
    }


    //_sparkSystem->setCameraPosition( cameraNode->getTranslation());


    // ->GetWorldTransform().Data());
    _sparkSystem->updateParticles(dt * 0.001f);
}

unsigned int SparkParticleEmitter::draw(bool wireframe)
{
    _sparkSystem->renderParticles();
    return 1;
}

}
