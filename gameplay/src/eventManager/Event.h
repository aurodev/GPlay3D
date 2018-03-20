#ifndef EVENT_H
#define EVENT_H

#include <cassert>
#include <vector>
#include <memory>

#include "EventParams.h"
#include "EventHandler.h"

namespace gameplay {

class Event
{
    typedef std::vector<EventBase* > EventHandleVec;

    // List of abstract event handlers
    EventHandleVec _callbackVec;

public:

    // Cleanup our event handlers
    ~Event();

    // Create an event handler for this callback
    template <class T>
    void addListener(T* spInst, void(T::*func)(EventParams& args))
    {
        EventHandler<T>* handle(new EventHandler<T>(spInst, func));
        _callbackVec.push_back(handle);
    }

    // Remove references to a specific callback instance
    void removeListener(void* pInst);

    // Number of callbacks for this event
    size_t count() const { return _callbackVec.size(); }

    // Functor to trigger the event callbacks
    void operator()(EventParams& args);
};

}

#endif
