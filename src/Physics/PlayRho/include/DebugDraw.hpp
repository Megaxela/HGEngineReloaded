#pragma once

#include <PlayRho/PlayRho.hpp>
#include <Color.hpp>
#include <Gizmos.hpp>

namespace PLAYRHO_PHYSICS_MODULE_NS
{
    /**
     * @brief Class, that incapsulates debug drawing to gizmos system.
     */
    class DebugDraw
    {
    public:
        using size_type = std::size_t;

        explicit DebugDraw(RENDERING_BASE_MODULE_NS::Gizmos* gizmos);

        /// Draw a closed polygon provided in CCW order.
        void drawPolygon(const playrho::Length2 *vertices,
                         size_type vertexCount,
                         const UTILS_MODULE_NS::Color &color);

        /// Draw a solid closed polygon provided in CCW order.
        void drawSolidPolygon(const playrho::Length2 *vertices,
                              size_type vertexCount,
                              const UTILS_MODULE_NS::Color &color);

        /// Draw a circle.
        void drawCircle(const playrho::Length2 &center,
                        playrho::Length radius,
                        const UTILS_MODULE_NS::Color &color);

        /// Draw a solid circle.
        void drawSolidCircle(const playrho::Length2 &center,
                             playrho::Length radius,
                             const UTILS_MODULE_NS::Color &color);

        /// Draw a line segment.
        void drawSegment(const playrho::Length2 &p1,
                         const playrho::Length2 &p2,
                         const UTILS_MODULE_NS::Color &color);

        /// Draw point
        void drawPoint(const playrho::Length2 &p,
                       playrho::Length size,
                       const UTILS_MODULE_NS::Color &color);

        /// Draws a string at the given screen coordinates.
        void drawString(int x, int y, const char *string, ...);

        /// Draws a string at the given world coordinates.
        void drawString(const playrho::Length2 &p, const char *string, ...);

    private:

        RENDERING_BASE_MODULE_NS::Gizmos* m_gizmos;

        float m_z;
    };
}