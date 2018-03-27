#include "SamplesGame.h"
#include "renderer/BGFXRenderer.h"
#include "FirstPersonCamera.h"

/**
 * Load sponza scene.
 */
class Sponza : public Example
{
    Scene* _scene;
    Font* _font;
    FirstPersonCamera _fpCamera;

public:

    Sponza()
        : _font(nullptr)
        , _scene(nullptr)
    {
    }

    void initialize()
    {
        // Create the font for drawing the framerate.
        _font = Font::create("res/coredata/ui/arial.gpb");

        // Load sponza scene
        _scene = Scene::load("res/data/samples/examples/sponza/sponza.gpb");

        // set fps camera
        Vector3 cameraPosition(0, 1, 10);
        _fpCamera.initialize(1.0, 100000.0f);
        _fpCamera.setPosition(cameraPosition);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());

        // Update the aspect ratio for our scene's camera to match the current device resolution
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
    }

    void finalize()
    {
        SAFE_RELEASE(_scene);
        SAFE_RELEASE(_font);
    }

    void update(float elapsedTime)
    {
        // update camera
        _fpCamera.updateCamera(elapsedTime);
    }

    void render(float elapsedTime)
    {
        _scene->visit(this, &Sponza::drawScene);

        // draw frame rate
        drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    }

    bool drawScene(Node* node)
    {
        Drawable* drawable = node->getDrawable();
        if (drawable)
            drawable->draw();
        return true;
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
ADD_SAMPLE("Graphics", "Sponza", Sponza, 4)
#endif
