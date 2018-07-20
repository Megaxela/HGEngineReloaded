#pragma once

#include <ostream>

namespace UTILS_MODULE_NS
{
    class Rect
    {
        friend std::ostream& operator<<(std::ostream& stream, const UTILS_MODULE_NS::Rect& r)
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
        bool operator==(const Rect& rhs);

        bool operator!=(const Rect& rhs);

        int x;
        int y;
        int w;
        int h;
    };
}

