#include "gameplay.h"
#include "Example.h"
#include "SamplesGame.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>
#include <eventManager/EventManager.h>
#include <io/FileWatcher.h>


using namespace gameplay;

/**
 * Base sample
 */
class ShaderReload : public Example
{
    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;
    Model* _cubeModel;



public:

    ShaderReload()
        : _font(nullptr), _scene(nullptr)
    {
    }    

    void finalize()
    {
        SAFE_RELEASE(_font);
        SAFE_RELEASE(_scene);
    }

    void onShaderDirectoryEvent(EventDataRef eventData)
    {
        auto watchFileEvent = std::dynamic_pointer_cast<FileWatcherEvent>(eventData);

        if(watchFileEvent)
        {

            // get event parameters and print log
            unsigned int action = watchFileEvent->_data.action;
            std::string dir = watchFileEvent->_data.directory;
            std::string filename = watchFileEvent->_data.filename;
            std::string oldFilename = watchFileEvent->_data.oldFilename;
            const char* actionLabel[] = { "Unknow", "Added", "Deleted", "Modified", "Moved" };
            print("onDirectoryEvent: type[%s] dir[%s] file[%s]\n", actionLabel[action], dir.c_str(), filename.c_str());

            // if event occurs in shader directory, reload cube material
            if(dir == "res/shaders")
            {
                // reload material of cubemodel
                _cubeModel->getMaterial()->reload();
            }
        }
    }

    void initialize()
    {
        // Create the font for drawing the framerate.
        _font = Font::create("res/ui/arial.gpb");

        // Create a new empty scene.
        _scene = Scene::create();

        // set fps camera
        Vector3 cameraPosition(0, 1, 5);
        _fpCamera.initialize(0.1f, 100.0f);
        _fpCamera.setPosition(cameraPosition);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());


        // Create a material.
        Material* material = Material::create("res/shaders/colored.vert", "res/shaders/colored.frag");
        material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
        material->getStateBlock()->setCullFace(true);
        material->getStateBlock()->setDepthTest(true);
        material->getStateBlock()->setDepthWrite(true);

        // Load box mesh.
        Bundle* bundle = Bundle::create("res/common/box.gpb");
        Mesh* meshBox = bundle->loadMesh("box_Mesh");
        SAFE_RELEASE(bundle);

        // Create a model from mesh
        _cubeModel = Model::create(meshBox);
        _cubeModel->setMaterial(material);

        // Attach model to a node and add it to scene
        Node* cubeNode = Node::create("myCube");
        cubeNode->setDrawable(_cubeModel);
        cubeNode->setScale(1.0f, 1.0f, 1.0f);
        _scene->addNode(cubeNode);




        // create views
        Game * game = Game::getInstance();
        View defaultView;
        defaultView.clearColor = 0x111122ff;
        defaultView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
        defaultView.depth = 1.0f;
        defaultView.stencil = 0;
        defaultView.rectangle = Rectangle(game->getWidth(), game->getHeight());
        game->insertView(0, defaultView);


        // Add to the fileWatcher a task to monitor the shaders directory.
        FileWatcher::Get()->addDirectory("res/shaders", true);

        // Add a listener to call specified metod when a file operation is detected in the shader directory
        EventManager::get()->addListener(GP_EVENT_LISTENER(this, ShaderReload::onShaderDirectoryEvent),
                                         FileWatcherEvent::E_FILEWATCHER);

    }

    void update(float elapsedTime)
    {
        // show toolbox
        showToolbox();

        // update camera
        _fpCamera.updateCamera(elapsedTime);
    }

    void render(float elapsedTime)
    {
        // render scene in main view
        Game::getInstance()->bindView(0);
        _scene->visit(this, &ShaderReload::drawScene);
        drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    }

    bool drawScene(Node* node)
    {
        Drawable* drawable = node->getDrawable();
        if (drawable)
            drawable->draw();
        return true;
    }

    void showToolbox()
    {
        bool p_open = true;
        const float DISTANCE = 10.0f;
        static int corner = 0;
        ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
        if (ImGui::Begin("Example: Fixed Overlay", &p_open, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoFocusOnAppearing|ImGuiWindowFlags_NoNav))
        {
            ImGui::Text("The shaders directory is curently watched.\nTry to edit res/shaders/colored.frag.\nSaving file will trigger an event that will rebuild the box material.");
            ImGui::End();
        }
    }

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        _fpCamera.touchEvent(evt, x, y, contactIndex);
    }

    void keyEvent(Keyboard::KeyEvent evt, int key)
    {
        _fpCamera.keyEvent(evt, key);
    }

};

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Shader hot reloading", ShaderReload, 9)
#endif
