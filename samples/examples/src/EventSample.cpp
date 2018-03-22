#include "gameplay.h"
#include "Example.h"
#include "SamplesGame.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>

//#include "eventManager/EventManager.h"



#include "eventManager/EventManager.h"



using namespace gameplay;


enum EventList
{
    EVENT_ACTOR_CREATE,
    EVENT_ACTOR_CREATED,
    EVENT_ACTOR_KILLED,
};








#include <cstdint>

namespace detail
{
    // FNV-1a 32bit hashing algorithm.
    constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
    {
        return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
    }
}    // namespace detail

constexpr std::uint32_t operator"" _hash(char const* s, std::size_t count)
{
    return detail::fnv1a_32(s, count);
}

constexpr std::uint32_t check = "0123456789ABCDEF"_hash;

















#include "eventManager/BaseEventData.h"

using MyEventDataRef = std::shared_ptr<class MyEventData>;


class MyEventData : public EventData
{
public:

    static MyEventDataRef create()
    {
        print("Actor::create.\n");
        return MyEventDataRef(new MyEventData());
    }

    EventDataRef copy()
    {

    }

    const char* getName() const
    {

    }

    static EventType TYPE;
    EventType getEventType() const
    {
        return TYPE;
    }

    void onEventCreateNewActor()
    {}

private:

    explicit MyEventData() :
        EventData(Platform::getAbsoluteTime())
    {

    }
};

EventType MyEventData::TYPE = "MyEventData"_hash;





///EventManager<EventList> _myEventManager;
//EventManagerRef mEventManager;

class Actor
{
public:

    Actor()
    {

    }

    void create()
    {
        print("Actor::create.\n");
        //@@_myEventManager.onEvent(EVENT_ACTOR_CREATED);
    }

    void destroy()
    {
        print("Actor::destroy.\n");
        //@@_myEventManager.onEvent(EVENT_ACTOR_KILLED);
    }


    void wouchEvent(EventDataRef eventData)
    {
        print("wouchEvent\n");
    }

};

using fastdelegate::MakeDelegate;




class EventSample : public Example
{
    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;
    Actor _actor;


public:

    EventSample()
        : _font(nullptr), _scene(nullptr)
    {

        //mEventManager = EventManager::create( "Global", true );





        EventListenerDelegate delegateFunc = MakeDelegate(this, &EventSample::onEventCreateNewActor);
        auto thisListenerDelegate = fastdelegate::MakeDelegate( this, &EventSample::onEventCreateNewActor );
        EventManager::get()->addListener( delegateFunc, MyEventData::TYPE );

        //EventManagerBase* pGlobalEventManager = EventManagerBase::get();
        const EventType sktype = "MyEventData"_hash;
        EventManager::get()->addListener(MakeDelegate(&_actor, &Actor::wouchEvent), sktype);


        //@@_myEventManager.registerEvent(EVENT_ACTOR_CREATE, this, &EventSample::onEventCreateNewActor);
        //@@_myEventManager.registerEvent(EVENT_ACTOR_CREATED, this, &EventSample::onEventActorCreated);

    }



    void onEventCreateNewActor(EventDataRef eventData)
    {
        //auto mouseEvent = std::dynamic_pointer_cast<MousePositionEvent>( eventData );

        auto mouseEvent = std::dynamic_pointer_cast<MyEventData>( eventData );





        print("sample => Create a new actor.\n");
        _actor.create();
    }


    /// void onEventCreateNewActor(EventParams& args)
    /// {
    ///     print("sample => Create a new actor.\n");
    ///
    ///     _actor.create();
    /// }
    ///
    ///
    /// void onEventActorCreated(EventParams& args)
    /// {
    ///     print("sample => an actor was created.\n");
    ///
    ///     _actor.destroy();
    /// }

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

        // Load box shape
        Bundle* bundle = Bundle::create("res/common/box.gpb");
        Node* nodeBox = bundle->loadNode("box");
        dynamic_cast<Model*>(nodeBox->getDrawable())->setMaterial("res/common/box.material", 0);
        SAFE_RELEASE(bundle);

        _scene->addNode(nodeBox);
    }

    void update(float elapsedTime)
    {
        // in my update I'll first update the event manager which if an event is queued
            // it will trigger for all listeners
        //mEventManager->update();


        // show toolbox
        showToolbox();


        //print("%f\n", Platform::getElapsedTime());

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
            ///_myEventManager.onEvent(EVENT_ACTOR_CREATE);
            ///
            ///

            EventManager::get()->queueEvent(MyEventData::create());
            //mEventManager->triggerThreadedEvent(MyEventData::create());


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

