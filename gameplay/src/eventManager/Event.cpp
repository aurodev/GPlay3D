#include "Event.h"

namespace gameplay {

Event::~Event()
{
    EventHandleVec::iterator it = _callbackVec.begin();

    while (it != _callbackVec.end()) {
        delete (*it);
        ++it;
    }

    _callbackVec.clear();
}

void Event::operator()(EventParams& args)
{
    EventHandleVec::iterator it = _callbackVec.begin();

    while (it != _callbackVec.end()) {
        (*it)->fire(args);
        ++it;
    }
}

void Event::removeListener(void* pInst)
{
    EventHandleVec::iterator it = _callbackVec.begin();

    while (it != _callbackVec.end())
    {
        if( (*it)->isEvent(pInst) ) {
            delete (*it);
            it = _callbackVec.erase(it);
        } else {
            ++it;
        }
    }
}

}
