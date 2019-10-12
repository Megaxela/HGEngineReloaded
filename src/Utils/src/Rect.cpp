// HG::Utils
#include <HG/Utils/Rect.hpp>

namespace HG::Utils
{
Rect::Rect() : x(0), y(0), w(0), h(0)
{
}

Rect::Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
{
}

bool Rect::operator==(const Rect& rhs) const
{
    return x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h;
}

bool Rect::operator!=(const Rect& rhs) const
{
    return x != rhs.x || y != rhs.y || w != rhs.w || h != rhs.h;
}
} // namespace HG::Utils
