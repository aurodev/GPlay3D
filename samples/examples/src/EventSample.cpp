#include "gameplay.h"
#include "Example.h"
#include "SamplesGame.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>

#include "events/EventManager.h"
#include "events/BaseEventData.h"

using namespace gameplay;



/**
 * Declare an event.
 *
 * This event store a Vector2 for mouse position.
 * And is intended to be triggered when mouse is clicked.
 */
using MyMouseEventRef = std::shared_ptr<class MyMouseEvent>;
class MyMouseEvent : public EventData
{
public:

    GP_DECLARE_EVENT(MyMouseEvent)

    Vector2 _mousePos;

    static MyMouseEventRef create(Vector2 mousePos)
    {
        print("MyMouseEvent::create\n");
        return MyMouseEventRef(new MyMouseEvent(mousePos));
    }

    ~MyMouseEvent()
    {
        print("MyMouseEvent::Destructor.\n");
    }

private:

    explicit MyMouseEvent(Vector2 mousePos) :
        EventData()
      , _mousePos(mousePos)
    {
        print("MyMouseEvent::Constructor\n");
    }
};




/**
 * An actor that wants to be notified by event.
 */
class DummyActor
{
public:

    DummyActor()
    {
        _pos = Vector2(0,0);
    }

    void onEventMouseClicked(EventDataRef eventData)
    {
        // this method was declared in initialisation as a listener for the MyMouseEvent event.

        // MyMouseEvent have been queued or triggered.
        // dynamic_cast the shared_ptr to get access to MyMouseEvent data.
        auto mouseEvent = std::dynamic_pointer_cast<MyMouseEvent>(eventData);

        print("DummyActor: oh the mouse was clicked at (%.0f,%.0f) !!!\n", mouseEvent->_mousePos.x, mouseEvent->_mousePos.y);

        // Do something usefull with that.

        _pos = mouseEvent->_mousePos;
    }

    void update()
    {
        ImGui::SetNextWindowPos(ImVec2(_pos.x, _pos.y));
        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoFocusOnAppearing|ImGuiWindowFlags_NoNav);
        ImGui::Text("Hello cube, I'm DummyActor");
        ImGui::End();
    }

private:

    Vector2 _pos;

};






/**
 * Sample to demonstrate events communication system.
 */
class EventSample : public Example
{
    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;
    Node* _originalBox;
    DummyActor _dummyActor;

public:

    EventSample()
        : _font(nullptr), _scene(nullptr)
    {
    }    

    void finalize()
    {
        // Remove the delegates from the event manager
        EventManager::get()->removeListener(GP_EVENT_LISTENER(this, EventSample::onEventCreateNewActor), MyMouseEvent::ID());
        EventManager::get()->removeListener(GP_EVENT_LISTENER(&_dummyActor, DummyActor::onEventMouseClicked), MyMouseEvent::ID());

        SAFE_RELEASE(_font);
        SAFE_RELEASE(_scene);
        SAFE_RELEASE(_originalBox);
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


        // Load a box shape and create the original box node but don't add it to scene.
        // It will be used as base node for creating clones.
        Bundle * bundle = Bundle::create("res/common/box.gpb");
        _originalBox = bundle->loadNode("box");
        dynamic_cast<Model*>(_originalBox->getDrawable())->setMaterial("res/common/box.material", 0);
        SAFE_RELEASE(bundle);

        // Add some listeners for the event MyMouseEvent.
        // This event is triggered in touchEvent() when a button is clicked.
        // Any object that wants to be notified of this event can add a listener.
        EventManager::get()->addListener(GP_EVENT_LISTENER(this, EventSample::onEventCreateNewActor), MyMouseEvent::ID());
        EventManager::get()->addListener(GP_EVENT_LISTENER(&_dummyActor, DummyActor::onEventMouseClicked), MyMouseEvent::ID());
    }


    Vector3 screenToWorld(float x, float y, float z, Rectangle viewport)
    {
        Vector3 p = Vector3(x, y, z);

        // get the world-space position at the near clip plane.
        Vector3 p0;
        _scene->getActiveCamera()->unproject(viewport, x, y, 0.0f, &p0);

        // get the world-space position at the far clip plane.
        Vector3 p1;
        _scene->getActiveCamera()->unproject(viewport, x, y, 1.0f, &p1);

        // find (x, y) coordinates
        float t = (z - p0.z) / (p1.z - p0.z);
        p.x = (p0.x + t * (p1.x - p0.x));
        p.y = (p0.y + t * (p1.y - p0.y));
        p.z = z;

        return p;
    }

    void onEventCreateNewActor(EventDataRef eventData)
    {
        // this method was declared in initialisation as a listener for the MyMouseEvent event.

        // MyMouseEvent have been queued or triggered.
        // dynamic_cast the shared_ptr to get access to MyMouseEvent data.
        auto mouseEvent = std::dynamic_pointer_cast<MyMouseEvent>(eventData);

        if(!mouseEvent)
            return;

        // convert 2D mouse coordinates to 3D world position, with z set as 0.0f;
        Vector3 pos3D = screenToWorld(mouseEvent->_mousePos.x,
                                      mouseEvent->_mousePos.y,
                                      0.0f,
                                      getViewport()
                                      );

        // clone the box and add it to scene.
        Node* newBox = _originalBox->clone();
        newBox->setTranslation(pos3D);
        _scene->addNode(newBox);

        print("onEventCreateNewActor => Create a new actor at mousePos(%.0fx%.0f) : 3DPos(%f,%f,%f)\n",
              mouseEvent->_mousePos.x,
              mouseEvent->_mousePos.y,
              pos3D.x,
              pos3D.y,
              pos3D.z);
    }

    void update(float elapsedTime)
    {
        // show toolbox
        showToolbox();

        _dummyActor.update();

        // update camera
        _fpCamera.updateCamera(elapsedTime);
    }

    void render(float elapsedTime)
    {
        // Clear the color and depth buffers
        clear(CLEAR_COLOR_DEPTH, 0.1f, 0.1f, 0.2f, 1.0f, 1.0f, 0);

        // Visit all the nodes in the scene, drawing the models.
        _scene->visit(this, &EventSample::drawScene);

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

            // Send event that mouse was clicked, don't care about if someone is waiting this event or not.
            EventManager::get()->queueEvent(MyMouseEvent::create(Vector2(x,y)));

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
    ADD_SAMPLE("Events", "Event demo", EventSample, 255);
#endif

