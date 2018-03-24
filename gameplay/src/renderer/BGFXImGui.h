#include "../core/Singleton.h"
#include "../core/Base.h"
#include "../input/Mouse.h"
#include <imgui/imgui.h>

namespace gameplay {

class BGFXImGui
{
public:
    void imguiRender(ImDrawData* drawData);
    void imguiShutdown();
    void imguiInit();
    void imguiReset(uint16_t width, uint16_t height);

private:
    bgfx::VertexDecl imguiVertexDecl;
    bgfx::TextureHandle imguiFontTexture;
    bgfx::UniformHandle imguiFontUniform;
    bgfx::ProgramHandle imguiProgram;
};

typedef Singleton<BGFXImGui> GPImGui;

}
