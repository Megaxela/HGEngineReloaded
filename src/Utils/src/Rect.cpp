#include "Rect.hpp"

UTILS_MODULE_NS::Rect::Rect() :
    x(0),
    y(0),
    w(0),
    h(0)
{

}

UTILS_MODULE_NS::Rect::Rect(int x, int y, int w, int h) :
    x(x),
    y(y),
    w(w),
    h(h)
{

}

bool UTILS_MODULE_NS::Rect::operator==(const UTILS_MODULE_NS::Rect& rhs)
{
    return x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h;
}

bool UTILS_MODULE_NS::Rect::operator!=(const UTILS_MODULE_NS::Rect& rhs)
{
    return x != rhs.x || y != rhs.y || w != rhs.w || h != rhs.h;
}