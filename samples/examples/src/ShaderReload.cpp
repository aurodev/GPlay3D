#include "gameplay.h"
#include "Example.h"
#include "SamplesGame.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>


#include "efsw/efsw.hpp"
#include <functional>
#include "Singleton.h"

using namespace gameplay;

class FileWatcherBase : public efsw::FileWatchListener
{

public:

    FileWatcherBase()
    {
        _fileWatcher = new efsw::FileWatcher();
        _fileWatcher->watch();
    }

    template <class T>
    void add(const char* directory, T* ptr, void (T::*handleWatchAction)(), bool recursive);

    void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "" )
    {
        // call binded method
        _list[watchid]();
    }

private:

    efsw::FileWatcher* _fileWatcher;
    std::map<efsw::WatchID, std::function<void()>> _list;
};
typedef Singleton<FileWatcherBase> FileWatcher;


template <class T>
void FileWatcherBase::add(const char* directory, T* ptr, void (T::*handleWatchAction)(), bool recursive)
{
    GP_ASSERT(_fileWatcher);

    //_fileWatcher->watch();

    // add a watch directory
    efsw::WatchID watchID = _fileWatcher->addWatch(directory, this, recursive);

    // bind watchID to the method to call and record it in map.
    _list[watchID] = std::bind(&T::handleWatchAction, ptr);
}




/**
 * Base sample
 */
class ShaderReload : public Example//, public efsw::FileWatchListener
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

    void handleWatchAction()
    {
        _cubeModel->getMaterial()->reload();
    }

    /*void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "" )
    {
        switch( action )
        {
        case efsw::Actions::Add:
            print("DIR[%s] FILE[%s] has event Added.", dir.c_str(), filename.c_str());
            break;
        case efsw::Actions::Delete:
            print("DIR[%s] FILE[%s] has event Delete.", dir.c_str(), filename.c_str());
            break;
        case efsw::Actions::Modified:
            print("DIR[%s] FILE[%s] has event Modified.", dir.c_str(), filename.c_str());
            _cubeModel->getMaterial()->reload();
            break;
        case efsw::Actions::Moved:
            print("DIR[%s] FILE[%s] has event Moved from [%s].", dir.c_str(), filename.c_str(), oldFilename.c_str());
            break;
        default:
            print("Should never happen.");
        }
    }*/


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


        // Add a fileWatcher to the specified directory and the method to call when a watch event happens.
        FileWatcher::Get()->add("res/shaders", this, &ShaderReload::handleWatchAction, true);



      /// // Create the file system watcher instance
      /// // efsw::FileWatcher allow a first boolean parameter that indicates if it should start with
      /// // the generic file watcher instead of the platform specific backend
      /// efsw::FileWatcher * fileWatcher = new efsw::FileWatcher();
      ///
      /// // Create the instance of your efsw::FileWatcherListener implementation
      /// //UpdateListener * listener = new UpdateListener();
      ///
      /// // Add a folder to watch, and get the efsw::WatchID
      /// // It will watch the /tmp folder recursively ( the third parameter indicates that is recursive )
      /// // Reporting the files and directories changes to the instance of the listener
      /// efsw::WatchID watchID = fileWatcher->addWatch( "res/shaders", this, true );
      ///
      /// // Adds another directory to watch. This time as non-recursive.
      /// //efsw::WatchID watchID2 = fileWatcher->addWatch( "/usr", listener, false );
      ///
      /// // Start watching asynchronously the directories
      /// fileWatcher->watch();
      ///
      /// // Remove the second watcher added
      /// // You can also call removeWatch by passing the watch path
      /// // ( it must end with an slash or backslash in windows, since that's how internally it's saved )
      /// //fileWatcher->removeWatch( watchID2 );

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
        ImGui::Begin("Toolbox");
        ImGui::End();
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
    ADD_SAMPLE("Graphics", "ShaderReload", ShaderReload, 9)
#endif
