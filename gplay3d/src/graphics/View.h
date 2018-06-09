#ifndef VIEW_H
#define VIEW_H

#include "../math/Rectangle.h"

namespace gameplay {

class View
{
public:

    enum class ClearFlags : unsigned short
    {
        NONE    = 0x0000,
        COLOR   = 0x0001,
        DEPTH   = 0x0002,
        STENCIL = 0x0004,
        COLOR_DEPTH = COLOR | DEPTH,
        COLOR_STENCIL = COLOR | STENCIL,
        DEPTH_STENCIL = DEPTH | STENCIL,
        COLOR_DEPTH_STENCIL = COLOR | DEPTH | STENCIL
    };


    static View* create(unsigned short id, Rectangle rectangle, ClearFlags clearFlags, unsigned int color = 0x00000000,
                float depth = 1.0f, unsigned char stencil = 0);

    static View* getView(unsigned short id);


    void bind();

    static unsigned short getCurrentViewId();

private:

    View();

    unsigned short _id;
    Rectangle _rectangle;
    ClearFlags _clearFlags;
    unsigned int _clearColor;
    float _depth;
    unsigned char _stencil;
    const char* _name;
    static unsigned short _curentViewId;
};

}

#endif // VIEW_H
