#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <cassert>

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "EventParams.h"
#include "Event.h"
#include "EventHandler.h"

namespace gameplay {

// An EventManager can store event keys in a specific type
template<class KeyType>
class EventManager
{
    typedef typename std::map<KeyType, Event> MappedEvents;
    typedef typename MappedEvents::iterator MappedEventsIt;
    MappedEvents _events;

public:
    // Check that there is an event of this type registered
    bool hasEvent(const KeyType& rEventKey) const
    {
        return _events.find(rEventKey) != _events.end();
    }

    // Trigger an event determined by the key (no params)
    void onEvent(const KeyType& rEventKey)
    {
        EventParams no_params;
        onEvent(rEventKey, no_params);
    }

    // Triggers an event determined by the key
    void onEvent(const KeyType& rEventKey, EventParams& args)
    {
        MappedEventsIt itResult(_events.find(rEventKey));

        // Checking the event exists as an early-out condition
        if (itResult != _events.end()) {
            (*itResult).second(args);
        }
    }

    // Register a new listener based on a class instance (static or dynamic)
    template <class EventT>
    void registerEvent(const KeyType& rEventKey, EventT* spInst, void (EventT::*func)(EventParams& args))
    {
        assert(spInst);

        // Our map will automatically add a non-existent key
        _events[rEventKey].template addListener<EventT>(spInst, func);
    }

    // Unregister a listener from all registered callbacks (optionally clear empty events)
    template <class EventT>
    void unregisterInstance(EventT* pInst, bool bClearOldEvents = true)
    {
        MappedEventsIt it(_events.begin());

        while( it != _events.end() ) {
            (*it).second.RemoveListener(pInst);

            // If there are no callbacks registered, clear the event
            if( bClearOldEvents && (*it).second.Count() == 0 ) {
                it = _events.erase(it);
            } else {
                ++it;
            }
        }
    }

};

}

#endif
