#pragma once

// C++ STL
#include <ostream>

namespace HG::Utils
{
class Rect
{
    friend std::ostream& operator<<(std::ostream& stream, const HG::Utils::Rect& r)
    {
        stream << "Rect(x=" << r.x << ", y=" << r.y << " " << r.w << "x" << r.h << ')';

        return stream;
    }

public:
    /**
     * @brief Default constructor;
     */
    Rect();

    /**
     * @brief Constructor.
     * @param x X position.
     * @param y Y position.
     * @param w Width.
     * @param h Height.
     */
    Rect(int x, int y, int w, int h);

    /**
     * Comparison operator.
     * @param rhs Right hand structure.
     * @return Is structs equal.
     */
    bool operator==(const Rect& rhs) const;

    bool operator!=(const Rect& rhs) const;

    int x;
    int y;
    int w;
    int h;
};
} // namespace HG::Utils
