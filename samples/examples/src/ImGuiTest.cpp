#include "ImGuiTest.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "ImGui", ImGuiTest, 7);
#endif


#include "BGFX/BGFXImGui.h"


#if 0

#include "Renderer.h"

#include <imgui/imgui.h>
#include <brtshaderc/brtshaderc.h>

#include <bx/math.h>


    template <class T>
    class GPSingleton
    {
    public:
        static T * Get()
        {
            static T instance;
            return &instance;
        }

    protected:
        GPSingleton() {}
        ~GPSingleton() {}

    private:
        GPSingleton(const GPSingleton &) {}
        GPSingleton &operator=(const GPSingleton &) { return *this; }
    };


    class BGFXImGui
    {
    public:
    void                imguiRender( ImDrawData* drawData );
    void                imguiShutdown();
    const char*         imguiGetClipboardText( void* userData );
    void                imguiSetClipboardText( void* userData, const char* text );

    void imguiInit();
    void imguiReset( uint16_t width, uint16_t height );

    private:
    bgfx::VertexDecl    imguiVertexDecl;
    bgfx::TextureHandle imguiFontTexture;
    bgfx::UniformHandle imguiFontUniform;
    bgfx::ProgramHandle imguiProgram;

    };


    typedef GPSingleton<BGFXImGui> GPImGui;


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

        //vs_ocornut_imgui();
        //bgfx::ShaderHandle vs = bgfx::createShader( bgfx::makeRef( vs_ocornut_imgui(), vs_ocornut_imgui_len() ) );
        //bgfx::ShaderHandle fs = bgfx::createShader( bgfx::makeRef( fs_ocornut_imgui(), fs_ocornut_imgui_len() ) );
        imguiProgram = bgfx::createProgram( vs, fs, true );

        // Setup render callback
        //io.RenderDrawListsFn = imguiRender;

        // Key mapping
        /*io.KeyMap[ ImGuiKey_Tab ] = GLFW_KEY_TAB;
        io.KeyMap[ ImGuiKey_LeftArrow ] = GLFW_KEY_LEFT;
        io.KeyMap[ ImGuiKey_RightArrow ] = GLFW_KEY_RIGHT;
        io.KeyMap[ ImGuiKey_UpArrow ] = GLFW_KEY_UP;
        io.KeyMap[ ImGuiKey_DownArrow ] = GLFW_KEY_DOWN;
        io.KeyMap[ ImGuiKey_PageUp ] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ ImGuiKey_PageDown ] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ ImGuiKey_Home ] = GLFW_KEY_HOME;
        io.KeyMap[ ImGuiKey_End ] = GLFW_KEY_END;
        io.KeyMap[ ImGuiKey_Delete ] = GLFW_KEY_DELETE;
        io.KeyMap[ ImGuiKey_Backspace ] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ ImGuiKey_Enter ] = GLFW_KEY_ENTER;
        io.KeyMap[ ImGuiKey_Escape ] = GLFW_KEY_ESCAPE;
        io.KeyMap[ ImGuiKey_A ] = GLFW_KEY_A;
        io.KeyMap[ ImGuiKey_C ] = GLFW_KEY_C;
        io.KeyMap[ ImGuiKey_V ] = GLFW_KEY_V;
        io.KeyMap[ ImGuiKey_X ] = GLFW_KEY_X;
        io.KeyMap[ ImGuiKey_Y ] = GLFW_KEY_Y;
        io.KeyMap[ ImGuiKey_Z ] = GLFW_KEY_Z;
        io.SetClipboardTextFn = imguiSetClipboardText;
        io.GetClipboardTextFn = imguiGetClipboardText;*/



        //ImGuiIO& io = ImGui::GetIO();

        io.DisplaySize = ImVec2(1280.0f, 720.0f);
        io.DeltaTime   = 1.0f / 60.0f;
        io.IniFilename = NULL;
    }

    void BGFXImGui::imguiReset( uint16_t width, uint16_t height )
    {
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







#endif











static Mesh* createTexturedCube(float size = 1.0f)
{
    struct PosColorVertex
    {
        Vector3 m_pos;
        Vector3 m_normal;
        Vector2 m_uv;
    };

    PosColorVertex vertices[] =
    {
           // position             // normal                       // texcoord
        {  Vector3(-1, -1,  1),    Vector3(  0.0,  0.0,  1.0),     Vector2(0.0, 0.0)  },
        {  Vector3( 1, -1,  1),    Vector3(  0.0,  0.0,  1.0),     Vector2(1.0, 0.0)  },
        {  Vector3(-1,  1,  1),    Vector3(  0.0,  0.0,  1.0),     Vector2(0.0, 1.0)  },
        {  Vector3( 1,  1,  1),    Vector3(  0.0,  0.0,  1.0),     Vector2(1.0, 1.0)  },
        {  Vector3(-1,  1,  1),    Vector3(  0.0,  1.0,  0.0),     Vector2(0.0, 0.0)  },
        {  Vector3( 1,  1,  1),    Vector3(  0.0,  1.0,  0.0),     Vector2(1.0, 0.0)  },
        {  Vector3(-1,  1, -1),    Vector3(  0.0,  1.0,  0.0),     Vector2(0.0, 1.0)  },
        {  Vector3( 1,  1, -1),    Vector3(  0.0,  1.0,  0.0),     Vector2(1.0, 1.0)  },
        {  Vector3(-1,  1, -1),    Vector3(  0.0,  0.0, -1.0),     Vector2(0.0, 0.0)  },
        {  Vector3( 1,  1, -1),    Vector3(  0.0,  0.0, -1.0),     Vector2(1.0, 0.0)  },
        {  Vector3(-1, -1, -1),    Vector3(  0.0,  0.0, -1.0),     Vector2(0.0, 1.0)  },
        {  Vector3( 1, -1, -1),    Vector3(  0.0,  0.0, -1.0),     Vector2(1.0, 1.0)  },
        {  Vector3(-1, -1, -1),    Vector3(  0.0, -1.0,  0.0),     Vector2(0.0, 0.0)  },
        {  Vector3( 1, -1, -1),    Vector3(  0.0, -1.0,  0.0),     Vector2(1.0, 0.0)  },
        {  Vector3(-1, -1,  1),    Vector3(  0.0, -1.0,  0.0),     Vector2(0.0, 1.0)  },
        {  Vector3( 1, -1,  1),    Vector3(  0.0, -1.0,  0.0),     Vector2(1.0, 1.0)  },
        {  Vector3( 1, -1,  1),    Vector3(  1.0,  0.0,  0.0),     Vector2(0.0, 0.0)  },
        {  Vector3( 1, -1, -1),    Vector3(  1.0,  0.0,  0.0),     Vector2(1.0, 0.0)  },
        {  Vector3( 1,  1,  1),    Vector3(  1.0,  0.0,  0.0),     Vector2(0.0, 1.0)  },
        {  Vector3( 1,  1, -1),    Vector3(  1.0,  0.0,  0.0),     Vector2(1.0, 1.0)  },
        {  Vector3(-1, -1, -1),    Vector3( -1.0,  0.0,  0.0),     Vector2(0.0, 0.0)  },
        {  Vector3(-1, -1,  1),    Vector3( -1.0,  0.0,  0.0),     Vector2(1.0, 0.0)  },
        {  Vector3(-1,  1, -1),    Vector3( -1.0,  0.0,  0.0),     Vector2(0.0, 1.0)  },
        {  Vector3(-1,  1,  1),    Vector3( -1.0,  0.0,  0.0),     Vector2(1.0, 1.0)  }
    };


    short indices[] =
    {
        0, 1, 2,
        2, 1, 3,
        4, 5, 6,
        6, 5, 7,
        8, 9, 10,
        10, 9, 11,
        12, 13, 14,
        14, 13, 15,
        16, 17, 18,
        18, 17, 19,
        20, 21, 22,
        22, 21, 23
    };

    unsigned int vertexCount = 24;
    unsigned int indexCount = 36;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, true);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setVertexData(vertices, 0, vertexCount);

    MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, true);
    meshPart->setIndexData(indices, 0, indexCount);
    return mesh;
}








ImGuiTest::ImGuiTest()
    : _font(NULL)
    , _triangleModel(NULL)
    , _cubeModel(NULL)
    , _spinDirection(-1.0f)
{    
}

void ImGuiTest::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    // Create a perspective projection matrix.
    Matrix projMatrix;
    Matrix::createPerspective(45.0f, getWidth() / (float)getHeight(), 1.0f, 100.0f, &projMatrix);

    // Create a lookat view matrix.
    Matrix viewMatrix;
    Matrix::createLookAt(Vector3(3,2,-5), Vector3::zero(), Vector3::unitY(), &viewMatrix);

    // set mvp matrix
    _worldViewProjectionMatrix = projMatrix * viewMatrix;

    // Create a material
    Material* material = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag");
    Texture::Sampler * sampler = Texture::Sampler::create("res/png/brick.png");
    material->getParameter("u_diffuseTexture")->setValue(sampler);
    material->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);
    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);

    // Create a cube.
    Mesh* meshQuad = createTexturedCube();
    _cubeModel = Model::create(meshQuad);
    _cubeModel->setMaterial(material);
    SAFE_RELEASE(meshQuad);








    // Set views

   /* Game * game = Game::getInstance();

    View defaultView;
    defaultView.clearColor = 0x111122ff;
    defaultView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
    defaultView.depth = 1.0f;
    defaultView.stencil = 0;
    defaultView.rectangle = Rectangle(game->getWidth(), game->getHeight());
    game->insertView(0, defaultView);

    View secondView;
    secondView.clearColor = 0xaaaaaaff;
    secondView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
    secondView.depth = 1.0f;
    secondView.stencil = 0;
    secondView.rectangle = Rectangle(200, 200);
    game->insertView(200, secondView);



*/

   // GPImGui::Get()->imguiInit();
   // GPImGui::Get()->imguiReset(getWidth(), getHeight());






}

void ImGuiTest::finalize()
{
    SAFE_RELEASE(_cubeModel);
    SAFE_RELEASE(_triangleModel);
    SAFE_RELEASE(_font);
}

void ImGuiTest::update(float elapsedTime)
{
    // Update the rotation of the cube.
    float dt = elapsedTime * 0.001f;
    float dx = _spinDirection * MATH_PI * 1.5f;
    float dy = _spinDirection * MATH_PI * 1.2f;
    float dz = _spinDirection * MATH_PI * 1.3f;
    Quaternion rot = Quaternion(Vector3(dx, dy, dz), dt);
    _worldViewProjectionMatrix.rotate(rot); 
}

void ImGuiTest::render(float elapsedTime)
{


    //Game::getInstance()->bindView(200);

    /*ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = elapsedTime;
    io.DisplaySize = ImVec2( ( float )getWidth(), ( float )getHeight() );*/

    ///ImGui::NewFrame();

    static float guiPixelateValue = 64.0f;
    ImGui::Begin("Controls");
    ImGui::SliderFloat("Pixelate", &guiPixelateValue, 12.0f, 128.0f);
    ImGui::End();

    ImGui::ShowTestWindow();


    ///ImGui::Render();
   //// GPImGui::Get()->imguiRender(ImGui::GetDrawData());



/*
    ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        // Build atlas
        unsigned char* tex_pixels = NULL;
        int tex_w, tex_h;
        io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

        for (int n = 0; n < 50; n++)
        {
            printf("NewFrame() %d\n", n);
            io.DisplaySize = ImVec2(1920, 1080);
            io.DeltaTime = 1.0f / 60.0f;
            ImGui::NewFrame();

            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::ShowDemoWindow(NULL);

            ImGui::Render();
    }

    imguiRender(ImGui::GetDrawData());
*/










   // bgfx::touch(0);

    //Game::getInstance()->bindView(0);

    // draw the cube
    _cubeModel->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);
    _cubeModel->draw();

    // draw frame rate
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());






}

void ImGuiTest::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        // Reverse the spin direction if the user touches the screen.
        _spinDirection *= -1.0f;
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}


