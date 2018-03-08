#include "BGFXImGui.h"

#include "../Base.h"
#include <imgui/imgui.h>
#include <brtshaderc/brtshaderc.h>
#include <bx/math.h>
#include <Keyboard.h>

namespace gameplay {


void BGFXImGui::imguiInit()
{
    unsigned char* data;
    int width, height;

    ImGuiContext* imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGuiIO& io = ImGui::GetIO();

    // Setup vertex declaration
    imguiVertexDecl
        .begin()
        .add( bgfx::Attrib::Position, 2, bgfx::AttribType::Float )
        .add( bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float )
        .add( bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true )
        .end();

    // Create font
    io.Fonts->AddFontDefault();
    io.Fonts->GetTexDataAsRGBA32( &data, &width, &height );
    imguiFontTexture = bgfx::createTexture2D( ( uint16_t )width, ( uint16_t )height, false, 1, bgfx::TextureFormat::BGRA8, 0, bgfx::copy( data, width*height * 4 ) );
    imguiFontUniform = bgfx::createUniform( "s_tex", bgfx::UniformType::Int1 );

    // Create shader program
    const bgfx::Memory* memVsh = shaderc::compileShader(shaderc::ST_VERTEX, "res/shaders/ocornut_imgui.vert", 0, "res/shaders/ocornut_imgui.io");
    const bgfx::Memory* memFsh = shaderc::compileShader(shaderc::ST_FRAGMENT, "res/shaders/ocornut_imgui.frag", 0, "res/shaders/ocornut_imgui.io");
    bgfx::ShaderHandle vs = bgfx::createShader(memVsh);
    bgfx::ShaderHandle fs = bgfx::createShader(memFsh);
    imguiProgram = bgfx::createProgram( vs, fs, true );

    // Setup render callback
    //io.RenderDrawListsFn = imguiRender;

    // Key mapping
    io.KeyMap[ ImGuiKey_Tab ] = Keyboard::KEY_TAB;
    io.KeyMap[ ImGuiKey_LeftArrow ] = Keyboard::KEY_LEFT_ARROW;
    io.KeyMap[ ImGuiKey_RightArrow ] = Keyboard::KEY_RIGHT_ARROW;
    io.KeyMap[ ImGuiKey_UpArrow ] = Keyboard::KEY_UP_ARROW;
    io.KeyMap[ ImGuiKey_DownArrow ] = Keyboard::KEY_DOWN_ARROW;
    io.KeyMap[ ImGuiKey_PageUp ] = Keyboard::KEY_PG_UP;
    io.KeyMap[ ImGuiKey_PageDown ] = Keyboard::KEY_PG_DOWN;
    io.KeyMap[ ImGuiKey_Home ] = Keyboard::KEY_HOME;
    io.KeyMap[ ImGuiKey_End ] = Keyboard::KEY_END;
    io.KeyMap[ ImGuiKey_Delete ] = Keyboard::KEY_DELETE;
    io.KeyMap[ ImGuiKey_Backspace ] = Keyboard::KEY_BACKSPACE;
    io.KeyMap[ ImGuiKey_Enter ] = Keyboard::KEY_KP_ENTER;
    io.KeyMap[ ImGuiKey_Escape ] = Keyboard::KEY_ESCAPE;
    io.KeyMap[ ImGuiKey_A ] = Keyboard::KEY_A;
    io.KeyMap[ ImGuiKey_C ] = Keyboard::KEY_C;
    io.KeyMap[ ImGuiKey_V ] = Keyboard::KEY_V;
    io.KeyMap[ ImGuiKey_X ] = Keyboard::KEY_X;
    io.KeyMap[ ImGuiKey_Y ] = Keyboard::KEY_Y;
    io.KeyMap[ ImGuiKey_Z ] = Keyboard::KEY_Z;
    /*io.SetClipboardTextFn = imguiSetClipboardText;
    io.GetClipboardTextFn = imguiGetClipboardText;*/



    //ImGuiIO& io = ImGui::GetIO();

    //io.DisplaySize = ImVec2(1280.0f, 720.0f);
    //io.DeltaTime   = 1.0f / 60.0f;
    //io.IniFilename = NULL;
}

void BGFXImGui::imguiReset( uint16_t width, uint16_t height )
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);
    io.DeltaTime   = 1.0f / 60.0f;
    io.IniFilename = NULL;


    //bgfx::setViewRect( 200, 0, 0, width, height );
    //bgfx::setViewClear( 0, BGFX_CLEAR_COLOR, 0x00000000 );
}

void BGFXImGui::imguiRender( ImDrawData* drawData )
{
    bgfx::ViewId m_viewId = 255;

    const ImGuiIO& io = ImGui::GetIO();
    const float width  = io.DisplaySize.x;
    const float height = io.DisplaySize.y;

    bgfx::setViewName(m_viewId, "ImGui");
    bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);



    const bgfx::HMD*  hmd  = bgfx::getHMD();
    const bgfx::Caps* caps = bgfx::getCaps();
    if (NULL != hmd && 0 != (hmd->flags & BGFX_HMD_RENDERING) )
    {
        float proj[16];
        bx::mtxProj(proj, hmd->eye[0].fov, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

        static float time = 0.0f;
        time += 0.05f;

        const float dist = 10.0f;
        const float offset0 = -proj[8] + (hmd->eye[0].viewOffset[0] / dist * proj[0]);
        const float offset1 = -proj[8] + (hmd->eye[1].viewOffset[0] / dist * proj[0]);

        float ortho[2][16];
        const float viewOffset = width/4.0f;
        const float viewWidth  = width/2.0f;
        bx::mtxOrtho(ortho[0], viewOffset, viewOffset + viewWidth, height, 0.0f, 0.0f, 1000.0f, offset0, caps->homogeneousDepth);
        bx::mtxOrtho(ortho[1], viewOffset, viewOffset + viewWidth, height, 0.0f, 0.0f, 1000.0f, offset1, caps->homogeneousDepth);
        bgfx::setViewTransform(m_viewId, NULL, ortho[0], BGFX_VIEW_STEREO, ortho[1]);
        bgfx::setViewRect(m_viewId, 0, 0, hmd->width, hmd->height);
    }
    else
    {
        float ortho[16];
        bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
        bgfx::setViewTransform(m_viewId, NULL, ortho);
        bgfx::setViewRect(m_viewId, 0, 0, uint16_t(width), uint16_t(height) );
    }








    for ( int ii = 0, num = drawData->CmdListsCount; ii < num; ++ii )
    {
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;

        const ImDrawList* drawList = drawData->CmdLists[ii];
        uint32_t numVertices = ( uint32_t )drawList->VtxBuffer.size();
        uint32_t numIndices  = ( uint32_t )drawList->IdxBuffer.size();

        if ( !bgfx::getAvailTransientVertexBuffer( numVertices, imguiVertexDecl ) || !bgfx::getAvailTransientIndexBuffer( numIndices ) )
        {
            break;
        }

        bgfx::allocTransientVertexBuffer( &tvb, numVertices, imguiVertexDecl );
        bgfx::allocTransientIndexBuffer( &tib, numIndices );

        ImDrawVert* verts = ( ImDrawVert* )tvb.data;
        memcpy( verts, drawList->VtxBuffer.begin(), numVertices * sizeof( ImDrawVert ) );

        ImDrawIdx* indices = ( ImDrawIdx* )tib.data;
        memcpy( indices, drawList->IdxBuffer.begin(), numIndices * sizeof( ImDrawIdx ) );

        uint32_t offset = 0;
        for ( const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd )
        {
            if ( cmd->UserCallback )
            {
                cmd->UserCallback( drawList, cmd );
            }
            else if ( 0 != cmd->ElemCount )
            {

                uint64_t state = 0
                        | BGFX_STATE_WRITE_RGB
                        | BGFX_STATE_WRITE_A
                        | BGFX_STATE_MSAA
                        ;

                bgfx::TextureHandle th = imguiFontTexture;
                if ( cmd->TextureId != NULL )
                {
                    union { ImTextureID ptr; struct { uint16_t flags; bgfx::TextureHandle handle; } s; } texture = { cmd->TextureId };
                    state |= BGFX_STATE_BLEND_FUNC( BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA );
                    th = texture.s.handle;
                }
                else
                {
                    state |= BGFX_STATE_BLEND_FUNC( BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA );
                }
                const uint16_t xx = uint16_t( fmax( cmd->ClipRect.x, 0.0f ) );
                const uint16_t yy = uint16_t( fmax( cmd->ClipRect.y, 0.0f ) );
                bgfx::setScissor( xx, yy, uint16_t( fmin( cmd->ClipRect.z, 65535.0f ) - xx ), uint16_t( fmin( cmd->ClipRect.w, 65535.0f ) - yy ) );
                bgfx::setState( state );
                bgfx::setTexture( 0, imguiFontUniform, th );
                bgfx::setVertexBuffer( 0, &tvb, 0, numVertices );
                bgfx::setIndexBuffer( &tib, offset, cmd->ElemCount );
                bgfx::submit(m_viewId, imguiProgram );
            }

            offset += cmd->ElemCount;
        }
    }


}

void BGFXImGui::imguiShutdown()
{
    bgfx::destroy( imguiFontUniform );
    bgfx::destroy( imguiFontTexture );
    bgfx::destroy( imguiProgram );
    ImGui::DestroyContext();
}

/* static const char* imguiGetClipboardText( void* userData )
{
    return glfwGetClipboardString( ( GLFWwindow* )userData );
}

static void imguiSetClipboardText( void* userData, const char* text )
{
    glfwSetClipboardString( ( GLFWwindow* )userData, text );
}*/






}
