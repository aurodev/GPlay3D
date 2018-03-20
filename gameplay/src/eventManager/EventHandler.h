#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <cassert>

#include "Event.h"
#include "EventBase.h"

namespace gameplay {

template <typename T>
class EventHandler : public EventBase
{
    typedef void (T::*TCallback)(EventParams& args);

    T* _pClassInst;
    TCallback _pCallbackFunc;
public:

    // Constructor which stores a pointer to a class instance, and the callback function
    EventHandler(T* pClassInst, TCallback cbFunction)
        : _pClassInst(pClassInst)
        , _pCallbackFunc(cbFunction)
    {
        assert(_pClassInst);
    }

    // Destructor
    virtual ~EventHandler()
    {
        _pClassInst = nullptr;
        _pCallbackFunc = nullptr;
    }

    // Search helper to find a specific pointer
    virtual bool isEvent(void* pInst) const
    {
        return (T*)pInst == _pClassInst;
    }

    // Call the callback function
    virtual void fire(EventParams& args)
    {
        if (_pClassInst) {
            (_pClassInst->*_pCallbackFunc)(args);
        }
    }
};

}

#endif
