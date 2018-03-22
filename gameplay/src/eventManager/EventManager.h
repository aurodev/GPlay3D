#pragma once

#include <deque>
#include <map>
#include <array>
#include <list>
#include <atomic>
#include <mutex>

#include "EventManagerBase.h"

const uint32_t NUM_QUEUES = 2u;
using EventManagerRef = std::shared_ptr<class EventManager>;


class EventManager : public EventManagerBase
{
    using EventListenerList = std::list<EventListenerDelegate>;
    using EventListenerMap	= std::map<EventType, EventListenerList>;
    using EventQueue		= std::deque<EventDataRef>;

public:

    static EventManagerRef create( const std::string &name, bool setAsGlobal );

    virtual ~EventManager();

    virtual bool addListener( const EventListenerDelegate &eventDelegate, const EventType &type ) override;
    virtual bool removeListener( const EventListenerDelegate &eventDelegate, const EventType &type ) override;

    virtual bool triggerEvent( const EventDataRef &event ) override;
    virtual bool queueEvent( const EventDataRef &event ) override;
    virtual bool abortEvent( const EventType &type, bool allOfType = false ) override;

    virtual bool addThreadedListener( const EventListenerDelegate &eventDelegate, const EventType &type ) override;
    virtual bool removeThreadedListener( const EventListenerDelegate &eventDelegate, const EventType &type ) override;
    virtual void removeAllThreadedListeners() override;
    virtual bool triggerThreadedEvent( const EventDataRef &event ) override;

    virtual bool update( uint64_t maxMillis = kINFINITE ) override;

private:
    explicit EventManager( const std::string &name, bool setAsGlobal );

    std::mutex mThreadedEventListenerMutex;
    EventListenerMap mThreadedEventListeners;

    EventListenerMap mEventListeners;
    std::array<EventQueue, NUM_QUEUES> mQueues;
    uint32_t mActiveQueue;

};
