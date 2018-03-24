#include "../core/Base.h"
#include "../graphics/FrameBuffer.h"
#include "../core/Game.h"
#include "../renderer/Renderer.h"

#define FRAMEBUFFER_ID_DEFAULT "org.gameplay3d.framebuffer.default"

namespace gameplay {


std::vector<FrameBuffer*> FrameBuffer::_frameBuffers;


FrameBuffer::FrameBuffer(const char *id) :
    _frameBufferHandle(BGFX_INVALID_HANDLE)
  , _id(id ? id : "")
{

}

FrameBuffer::~FrameBuffer()
{
    for (unsigned int i=0; i<_textures.size(); ++i)
    {
        if (_textures[i])
        {
            SAFE_RELEASE(_textures[i]);
        }
    }

    // Remove self from vector.
    std::vector<FrameBuffer*>::iterator it = std::find(_frameBuffers.begin(), _frameBuffers.end(), this);
    if (it != _frameBuffers.end())
    {
        _frameBuffers.erase(it);
    }
}

FrameBuffer* FrameBuffer::create(const char* id, uint16_t width, uint16_t height, Texture::Format format)
{
    uint32_t textureFlags = BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP;
    bgfx::TextureFormat::Enum bgfxFormat = BGFXTexture::toBgfxFormat(format);

    // create simple frame buffer
    FrameBuffer* frameBuffer = new FrameBuffer(id);
    frameBuffer->_frameBufferHandle = bgfx::createFrameBuffer(width, height, bgfxFormat, textureFlags);

    // create a texture to store framebuffer attachment using bgfx texture handle
    Texture* texture = Texture::create(format, width, height, 0, false, Texture::TEXTURE_RT);
    texture->_gpuTtexture->_handle = bgfx::getTexture(frameBuffer->_frameBufferHandle);
    frameBuffer->_textures.push_back(texture);

    // store this frame buffer into the list of availables frame buffers
    _frameBuffers.push_back(frameBuffer);

    return frameBuffer;
}

FrameBuffer* FrameBuffer::create(const char* id, std::vector<Texture*> textures)
{
    uint8_t num = (uint8_t)textures.size();

    // collect texture handles
    bgfx::TextureHandle* tmpTextureHandles = new bgfx::TextureHandle[num];
    for(size_t i=0; i<textures.size(); i++)
    {
        tmpTextureHandles[i] = textures[i]->getGpuTexture()->getHandle();
    }

    // create MRT frame buffer
    FrameBuffer* frameBuffer = new FrameBuffer(id);
    frameBuffer->_frameBufferHandle = bgfx::createFrameBuffer(num, tmpTextureHandles);
    frameBuffer->_textures = textures;

    delete tmpTextureHandles;

    // store this frame buffer into the list of availables frame buffers
    _frameBuffers.push_back(frameBuffer);

    return frameBuffer;
}

Texture* FrameBuffer::getRenderTarget(uint16_t id)
{
    GP_ASSERT(id >= 0);

    if(id <= _textures.size())
    {
       return _textures[id];
    }
    return nullptr;
}

Texture* FrameBuffer::getRenderTarget(std::string id)
{
    for(Texture* texture : _textures)
    {
        if(texture->getPath() == id)
            return texture;
    }
    return nullptr;
}

void FrameBuffer::bind()
{
    bgfx::setViewFrameBuffer(Game::getInstance()->_curentViewId, _frameBufferHandle);
}


FrameBuffer* FrameBuffer::getFrameBuffer(const char* id)
{
    GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<FrameBuffer*>::const_iterator it;
    for (it = _frameBuffers.begin(); it < _frameBuffers.end(); ++it)
    {
        FrameBuffer* fb = *it;
        GP_ASSERT(fb);
        if (strcmp(id, fb->getId()) == 0)
        {
            return fb;
        }
    }
    return NULL;
}

const char* FrameBuffer::getId() const
{
    return _id.c_str();
}

}
