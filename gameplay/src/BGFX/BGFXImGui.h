#include "../Singleton.h"

#include <Base.h>
#include <Mouse.h>
#include <imgui/imgui.h>

namespace gameplay {


class BGFXImGui
{
public:
    void imguiRender(ImDrawData* drawData);
    void imguiShutdown();
    const char* imguiGetClipboardText(void* userData);
    void imguiSetClipboardText(void* userData, const char* text);

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
