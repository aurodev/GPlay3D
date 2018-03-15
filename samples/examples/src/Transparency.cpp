#include "gameplay.h"
#include "Example.h"
#include "SamplesGame.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>

using namespace gameplay;

class Transparency : public Example
{
    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;

public:

    Transparency()
        : _font(nullptr), _scene(nullptr)
    {
    }

    void finalize()
    {
        SAFE_RELEASE(_font);
        SAFE_RELEASE(_scene);
    }

    void initialize()
    {
        // Create the font for drawing the framerate.
        _font = Font::create("res/ui/arial.gpb");

        // Create a new empty scene.
        _scene = Scene::create();

        // set fps camera
        Vector3 cameraPosition(0, 1, 5);
        _fpCamera.initialize(1.0, 10000.0f);
        _fpCamera.setPosition(cameraPosition);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

        // load box shape
        Bundle* bundle = Bundle::create("res/common/box.gpb");
        Node* nodeBox = bundle->loadNode("box");
        dynamic_cast<Model*>(nodeBox->getDrawable())->setMaterial("res/common/box.material", 0);
        SAFE_RELEASE(bundle);




        // create a plane
        Node* plane = nodeBox->clone();
        plane->setScale(8.0f, 0.001f, 8.0f);
        _scene->addNode(plane);

        // create 3 wide boxes
        nodeBox->setScale(4.0f, 1.0f, 0.2f);
        Node* box1 = nodeBox->clone();
        Node* box2 = nodeBox->clone();
        Node* box3 = nodeBox->clone();

        box2->rotate(Vector3(0,1,0), MATH_DEG_TO_RAD(30.0f));
        box3->rotate(Vector3(0,1,0), MATH_DEG_TO_RAD(-30.0f));

        _scene->addNode(box1);
        _scene->addNode(box2);
        _scene->addNode(box3);


        SAFE_RELEASE(nodeBox);
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
        // Clear the color and depth buffers
        clear(CLEAR_COLOR_DEPTH, 0.1f, 0.1f, 0.2f, 1.0f, 1.0f, 0);

        // Visit all the nodes in the scene, drawing the models.
        _scene->visit(this, &Transparency::drawScene);

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

        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            if (x < 75 && y < 50)
            {
                // Toggle Vsync if the user touches the top left corner
                setVsync(!isVsync());
            }
            break;
        case Touch::TOUCH_RELEASE:
            break;
        case Touch::TOUCH_MOVE:
            break;
        };
    }

    void keyEvent(Keyboard::KeyEvent evt, int key)
    {
        _fpCamera.keyEvent(evt, key);
    }

};

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Transparency", Transparency, 8);
#endif

