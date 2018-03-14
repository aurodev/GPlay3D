#pragma once

#include <spark/SPARK.h>
#include "../Base.h"
#include "../Ref.h"
#include "../Drawable.h"

namespace gameplay {

class Mesh;

class SparkParticleEmitter : public Ref, public Drawable
{
    friend class Node;

public:

    static SparkParticleEmitter* create(SPK::Ref<SPK::System> sparkSystem, bool worldTransformed);
    unsigned int draw(bool wireframe = false) override;
    void update(float dt);

private:

    /**
     * Constructor.
     */
    SparkParticleEmitter();

    /**
     * Destructor. Hidden use release() instead.
     */
    ~SparkParticleEmitter();

    /**
     * @see Drawable::clone
     */
    Drawable* clone(NodeCloneContext& context);


    void updateCameraPosition() const;


    SPK::Ref<SPK::System> _sparkSystem;
    bool _worldTransformed;
    bool _onlyWhenVisible;
    bool _alive;

};

}
