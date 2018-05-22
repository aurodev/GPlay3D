#ifndef PASS_H_
#define PASS_H_

#include "../graphics/RenderState.h"

namespace gameplay
{

class Technique;
class NodeCloneContext;

/**
 * Defines a pass for an object to be rendered.
 *
 * This class encapsulates the parameters and logic required to apply a shader
 * to an object to be rendered. This includes specifying both a vertex and fragment
 * shader, as well as any uniforms and vertex attributes to be applied to these.
 */
class Pass : public RenderState
{
    friend class Technique;
    friend class Material;
    friend class RenderState;

public:

    /**
     * Returns the Id of this pass.
     */ 
    const char* getId() const;

    /**
     * Returns the effect for this Pass.
     */
    Effect* getEffect() const;    

    /**
     * Binds the render state for this pass.
     *
     * This method should be called before executing any drawing code that should
     * use this pass. When drawing code is complete, the unbind() method should be called.
     */
    void bind(Mesh::PrimitiveType primitiveType);

    /**
     * Unbinds the render state for this pass.
     * 
     * This method should always be called when rendering for a pass is complete, to
     * restore the render state to the state it was in previous to calling bind().
     */
    void unbind();


    void setEffect(Effect* effect);

    bool reload();

    /**
     * Create a new Pass.
     *
     * @param id The pass id.
     * @param effect The effect to attach to this pass.
     *
     * @return The new created pass.
     */
    static Pass* create(Effect* effect, const char* id = nullptr);

private:

    /**
     * Constructor.
     */
    Pass(const char* id, Technique* technique);

    /**
     * Hidden copy constructor.
     */
    Pass(const Pass& copy);

    /**
     * Destructor.
     */
    ~Pass();

    /**
     * Creates a new pass for the given shaders.
     */
    bool initialize(const char* vshPath, const char* fshPath, const char* defines);

    /**
     * Hidden copy assignment operator.
     */
    Pass& operator=(const Pass&);

    /**
     * Clones the Pass and assigns it the given Technique.
     * 
     * @param technique The technique to assign to the new Pass.
     * @param context The clone context.
     * 
     * @return The newly created Pass.
     */
    Pass* clone(Technique* technique, NodeCloneContext &context) const;

    std::string _id;
    Technique* _technique;
    Effect* _effect;
};

}

#endif
