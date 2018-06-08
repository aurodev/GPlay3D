#include "../graphics/View.h"
#include "../renderer/BGFXRenderer.h"
#include <unordered_map>

namespace gameplay {

static std::unordered_map<unsigned short, View2*> __views;

View2::View2() :
  _id(0)
  , _rectangle(Rectangle())
  , _clearFlags(ClearFlags::NONE)
  , _clearColor(0x00000000)
  , _depth(1.0f)
  , _stencil(0)
  , _name(0)
{
    bgfx::setViewRect(_id, _rectangle.x, _rectangle.y, _rectangle.width, _rectangle.height);

    uint16_t clearFlags = static_cast<uint16_t>(_clearFlags);
    bgfx::setViewClear(_id, clearFlags, _clearColor, _depth, _stencil);
}


View2* View2::create(unsigned short id, Rectangle rectangle, ClearFlags clearFlags, unsigned int clearColor, float depth, unsigned char stencil)
{
    View2* view = new View2();
    view->_id = id;
    view->_rectangle = rectangle;
    view->_clearFlags = clearFlags;
    view->_clearColor = clearColor;
    view->_depth = depth;
    view->_stencil = stencil;

    __views.insert(std::make_pair(id, view));

    return view;
}

View2* View2::getView(unsigned short id)
{
    return __views[id];
}

}
