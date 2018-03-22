#pragma once

#include <memory>

using EventDataRef = std::shared_ptr<class EventData>;
using EventType = uint64_t;

class EventData
{
public:
    explicit EventData( float timestamp = 0.0f ) : mTimeStamp( timestamp ), mIsHandled( false ) {}
    virtual ~EventData() {}

    virtual EventDataRef copy() = 0;
    virtual const char* getName() const = 0;
    virtual EventType getEventType() const = 0;
    float getTimeStamp() { return mTimeStamp; }

    bool isHandled() { return mIsHandled; }
    void setIsHandled( bool handled = true ) { mIsHandled = handled; }

    //virtual void serialize( Buffer &streamOut ) = 0;
    //virtual void deSerialize( const Buffer &streamIn ) = 0;

private:
    const float mTimeStamp;
    bool mIsHandled;
};
