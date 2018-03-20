#include "EventParams.h"

namespace gameplay {

bool EventParams::find(const std::string& szParamName) const
{
    return _params.find(szParamName) != _params.end();
}

size_t EventParams::count() const
{
    return _params.size();
}

}
