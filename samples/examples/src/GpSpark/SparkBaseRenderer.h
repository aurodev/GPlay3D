#pragma once

#include <spark/SPARK_Core.h>
#include <spark/Core/SPK_Renderer.h>
#include "Material.h"
#include "SparkUtility.h"

namespace SPK {
namespace GP3D {

/**
 * The base renderer for all GPlay3D spark renderers
 */
class SparkBaseRenderer : public Renderer
{
public :

    virtual ~SparkBaseRenderer() {}

    virtual void setBlendMode(BlendMode blendMode) override;
   // void updateView(gameplay::Camera* camera);
    gameplay::Material* getMaterial();
    void setMaterial(gameplay::Material* material);

    void setViewMatrix(gameplay::Matrix viewMatrix) { _viewMatrix = viewMatrix; }

protected :

    SparkBaseRenderer(bool NEEDS_DATASET = false);  
    gameplay::Material* _material;
    gameplay::Matrix _viewMatrix;
};


}}
