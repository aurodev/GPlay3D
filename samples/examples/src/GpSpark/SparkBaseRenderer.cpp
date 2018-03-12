#include "SparkBaseRenderer.h"

namespace SPK {
namespace GP3D {

SparkBaseRenderer::SparkBaseRenderer(bool NEEDS_DATASET) :
    Renderer(NEEDS_DATASET)
{
    /*_camera = nullptr;
    _material = nullptr;*/
}

void SparkBaseRenderer::setBlendMode(BlendMode blendMode)
{
    switch(blendMode)
    {
    case BLEND_MODE_NONE :
        break;

    case BLEND_MODE_ADD :
        break;

    case BLEND_MODE_ALPHA :
        break;

    default :
        SPK_LOG_WARNING("Unsupported blending mode. Nothing happens");
        break;
    }
}

/*void SparkBaseRenderer::updateView(gameplay::Camera* camera)
{
    _camera = camera;
}*/

gameplay::Material* SparkBaseRenderer::getMaterial()
{
    return _material;
}

void SparkBaseRenderer::setMaterial(gameplay::Material* material)
{
    _material =  material;
}


}}
