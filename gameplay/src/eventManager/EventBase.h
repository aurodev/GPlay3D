#ifndef EVENTBASE_H
#define EVENTBASE_H

#include "EventParams.h"

namespace gameplay {

class EventBase
{
public:
    // Abstract call to trigger the event
    virtual ~EventBase() {}
    virtual void fire(EventParams& args) = 0;
    virtual bool isEvent(void* pInst) const = 0;
};

}

#endif
