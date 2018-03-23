#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "Base.h"
#include "RenderTarget.h"
#include "Image.h"

namespace gameplay
{


class FrameBuffer : public Ref
{
public:

     ~FrameBuffer();

    /// simple frame buffer with a single render target
    static FrameBuffer* create(const char* id, uint16_t width, uint16_t height, Texture::Format format);

    /// mrt frame buffer with multiple render target
    static FrameBuffer* create(const char *id, std::vector<Texture*> textures);

    void bind();
    Texture* getRenderTarget(uint16_t id);
    Texture* getRenderTarget(std::string id);


    /**
     * Get a named FrameBuffer from its ID.
     *
     * @param id The ID of the FrameBuffer to search for.
     *
     * @return The FrameBuffer with the specified ID, or NULL if one was not found.
     */
    static FrameBuffer* getFrameBuffer(const char* id);

    /**
     * Get the ID of this FrameBuffer.
     *
     * @return The ID of this FrameBuffer.
     */
    const char* getId() const;

private:
    FrameBuffer(const char* id);

    bgfx::FrameBufferHandle _frameBufferHandle;
    std::vector<Texture*> _textures;
    std::string _id;

    static std::vector<FrameBuffer*> _frameBuffers;
};

}

#endif
