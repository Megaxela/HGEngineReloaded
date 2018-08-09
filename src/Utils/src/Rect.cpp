#include "Rect.hpp"

HG::Utils::Rect::Rect() :
    x(0),
    y(0),
    w(0),
    h(0)
{

}

HG::Utils::Rect::Rect(int x, int y, int w, int h) :
    x(x),
    y(y),
    w(w),
    h(h)
{

}

bool HG::Utils::Rect::operator==(const HG::Utils::Rect& rhs)
{
    return x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h;
}

bool HG::Utils::Rect::operator!=(const HG::Utils::Rect& rhs)
{
    return x != rhs.x || y != rhs.y || w != rhs.w || h != rhs.h;
}