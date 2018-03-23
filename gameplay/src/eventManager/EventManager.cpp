#include <Base.h>
#include <Platform.h>
#include "EventManager.h"

//#define LOG_EVENT( stream )	GP_INFO( stream )
#define LOG_EVENT( stream )	((void)0)

EventManager::EventManager( const std::string &name, bool setAsGlobal )
    : EventManagerBase( name, setAsGlobal ), mActiveQueue( 0 )
{

}

EventManagerRef EventManager::create( const std::string &name, bool setAsGlobal )
{
    return EventManagerRef( new EventManager( name, setAsGlobal ) );
}

EventManager::~EventManager()
{
    GP_INFO( "Cleaning up event manager" );
    mEventListeners.clear();
    mQueues[0].clear();
    mQueues[1].clear();
    GP_INFO( "Removing all threaded events" );
    std::lock_guard<std::mutex> lock( mThreadedEventListenerMutex );
    mThreadedEventListeners.clear();
    GP_INFO( "Removed ALL EVENT LISTENERS" );
}

bool EventManager::addListener( const EventListenerDelegate &eventDelegate, const EventID &type )
{
    LOG_EVENT( "Attempting to add delegate function for event type: " + to_string( type ) );

    auto & eventDelegateList = mEventListeners[type];
    auto listenIt = eventDelegateList.begin();
    auto end = eventDelegateList.end();
    while ( listenIt != end )
    {
        if ( eventDelegate == (*listenIt) )
        {
            GP_WARN("Attempting to double-register a delegate");
            return false;
        }
        ++listenIt;
    }
    eventDelegateList.push_back(eventDelegate);
    GP_INFO("Successfully added delegate for event type: %s", std::to_string( type ) );
    return true;
}

bool EventManager::removeListener( const EventListenerDelegate &eventDelegate, const EventID &type )
{
    LOG_EVENT("Attempting to remove delegate function from event type: " + to_string( type ) );
    bool success = false;

    auto found = mEventListeners.find(type);
    if( found != mEventListeners.end() )
    {
        auto & listeners = found->second;
        for( auto listIt = listeners.begin(); listIt != listeners.end(); ++listIt )
        {
            if( eventDelegate == (*listIt) )
            {
                listeners.erase(listIt);
                LOG_EVENT("Successfully removed delegate function from event type: ");
                success = true;
                break;
            }
        }
    }
    return success;
}

bool EventManager::triggerEvent( const EventDataRef &event )
{
    //LOG_EVENT("Attempting to trigger event: " + std::string( event->getName() ) );
    bool processed = false;

    auto found = mEventListeners.find(event->getEventID());
    if( found != mEventListeners.end() )
    {
        const auto & eventListenerList = found->second;
        for( auto listIt = eventListenerList.begin(); listIt != eventListenerList.end(); ++listIt )
        {
            auto& listener = (*listIt);
            //LOG_EVENT("Sending event " + std::string( event->getName() ) + " to delegate.");
            listener( event );
            processed = true;
        }
    }

    return processed;
}

bool EventManager::queueEvent( const EventDataRef &event )
{
    GP_ASSERT(mActiveQueue < NUM_QUEUES);

    // make sure the event is valid
    if( !event )
    {
        GP_ERROR("Invalid event in queueEvent");
    }

    //	GP_INFO("Attempting to queue event: " + std::string( event->getName() ) );

    auto found = mEventListeners.find( event->getEventID() );
    if( found != mEventListeners.end() )
    {
        mQueues[mActiveQueue].push_back(event);
        LOG_EVENT("Successfully queued event: " + std::string( event->getName() ) );
        return true;
    }
    else
    {
        static bool processNotify = false;
        if( !processNotify )
        {
            LOG_EVENT( "Skipping event since there are no delegates to receive it: " + std::string( event->getName() ) );
            processNotify = true;
        }
        return false;
    }
}

bool EventManager::abortEvent( const EventID &type, bool allOfType )
{
    GP_ASSERT(mActiveQueue >= 0);
    GP_ASSERT(mActiveQueue > NUM_QUEUES);

    bool success = false;
    auto found = mEventListeners.find( type );

    if( found != mEventListeners.end() )
    {
        auto & eventQueue = mQueues[mActiveQueue];
        auto eventIt = eventQueue.begin();
        auto end = eventQueue.end();
        while( eventIt != end )
        {
            if( (*eventIt)->getEventID() == type )
            {
                eventIt = eventQueue.erase(eventIt);
                success = true;
                if( ! allOfType )
                    break;
            }
        }
    }

    return success;
}

bool EventManager::addThreadedListener( const EventListenerDelegate &eventDelegate, const EventID &type )
{
    std::lock_guard<std::mutex> lock( mThreadedEventListenerMutex );

    auto & eventDelegateList = mThreadedEventListeners[type];
    for ( auto & delegate : eventDelegateList )
    {
        if ( eventDelegate == delegate )
        {
            GP_WARN("Attempting to double-register a delegate");
            return false;
        }
    }
    eventDelegateList.push_back(eventDelegate);
    GP_INFO("Successfully added delegate for event type: %s", std::to_string( type ) );
    return true;
}

bool EventManager::removeThreadedListener( const EventListenerDelegate &eventDelegate, const EventID &type )
{
    std::lock_guard<std::mutex> lock( mThreadedEventListenerMutex );

    auto found = mThreadedEventListeners.find(type);
    if( found != mThreadedEventListeners.end() )
    {
        auto & listeners = found->second;
        for( auto listIt = listeners.begin(); listIt != listeners.end(); ++listIt )
        {
            if( eventDelegate == (*listIt) )
            {
                listeners.erase(listIt);
                LOG_EVENT("Successfully removed delegate function from event type: " << to_string( type ) );
                return true;
            }
        }
    }
    return false;
}

void EventManager::removeAllThreadedListeners()
{
    std::lock_guard<std::mutex> lock( mThreadedEventListenerMutex );
    mThreadedEventListeners.clear();
}

bool EventManager::triggerThreadedEvent( const EventDataRef &event )
{
    std::lock_guard<std::mutex> lock( mThreadedEventListenerMutex );

    bool processed = false;
    auto found = mThreadedEventListeners.find(event->getEventID());
    if( found != mThreadedEventListeners.end() )
    {
        const auto & eventListenerList = found->second;
        for( auto & listener : eventListenerList )
        {
            listener( event );
            processed = true;
        }
    }
#if ! defined(SHARINGSTATION)
    if( ! processed )
        GP_ERROR( "Tried triggering MultiThreaded Event without a listener" );
#endif
    return processed;
}

bool EventManager::update( uint64_t maxMillis )
{
    //@@uint64_t currMs = app::App::get()->getElapsedSeconds() * 1000;
    uint64_t currMs = gameplay::Platform::getAbsoluteTime();
    uint64_t maxMs = (( maxMillis == EventManager::kINFINITE ) ? (EventManager::kINFINITE) : (currMs + maxMillis) );

    int queueToProcess = mActiveQueue;
    mActiveQueue = (mActiveQueue + 1) % NUM_QUEUES;
    mQueues[mActiveQueue].clear();

    static bool processNotify = false;
    if( ! processNotify )
    {
        LOG_EVENT("Processing Event Queue " + to_string(queueToProcess) + "; " + to_string(mQueues[queueToProcess].size()) + " events to process");
        processNotify = true;
    }

    while (!mQueues[queueToProcess].empty())
    {
        auto event = mQueues[queueToProcess].front();
        mQueues[queueToProcess].pop_front();
        LOG_EVENT("\t\tProcessing Event " + std::string(event->getName()));

        const auto & eventType = event->getEventID();

        auto found = mEventListeners.find(eventType);
        if (found != mEventListeners.end())
        {
            const auto & eventListeners = found->second;
            LOG_EVENT("\t\tFound " + to_string(eventListeners.size()) + " delegates");

            auto listIt = eventListeners.begin();
            auto end = eventListeners.end();
            while (listIt != end)
            {
                auto listener = (*listIt);
                LOG_EVENT("\t\tSending Event " + std::string(event->getName()) + " to delegate");
                listener(event);
                listIt++;
            }
        }

        //@@currMs = app::App::get()->getElapsedSeconds() * 1000;//Engine::getTickCount();
        currMs = gameplay::Platform::getAbsoluteTime();
        if( maxMillis != EventManager::kINFINITE && currMs >= maxMs )
        {
            LOG_EVENT("Aborting event processing; time ran out");
            break;
        }
    }

    bool queueFlushed = mQueues[queueToProcess].empty();
    if( ! queueFlushed )
    {
        while( ! mQueues[queueToProcess].empty() )
        {
            auto event = mQueues[queueToProcess].back();
            mQueues[queueToProcess].pop_back();
            mQueues[mActiveQueue].push_front(event);
        }
    }

    return queueFlushed;
}
