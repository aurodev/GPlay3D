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


/// Helper macro
#define GP_EVENT_LISTENER(listener, method) (fastdelegate::MakeDelegate(listener, &method))


#include <cstdint>

///----------------------------------------------------------------------------------------------------
/// C++11 compile-time hash of literal strings.
///----------------------------------------------------------------------------------------------------
namespace detail
{
    // FNV-1a 32bit hashing algorithm.
    constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
    {
        return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
    }
}

constexpr std::uint32_t operator"" _hash(char const* s, std::size_t count)
{
    return detail::fnv1a_32(s, count);
}



