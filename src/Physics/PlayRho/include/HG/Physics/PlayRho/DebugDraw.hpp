#pragma once

// HG::Utils
#include <HG/Utils/Color.hpp>

// PlayRho
#include <PlayRho/PlayRho.hpp>

namespace HG::Rendering::Base
{
class Gizmos;
}

namespace HG::Physics::PlayRho
{
/**
 * @brief Class, that incapsulates debug drawing to gizmos system.
 */
class DebugDraw
{
public:
    using size_type = std::size_t;

    explicit DebugDraw(HG::Rendering::Base::Gizmos* gizmos);

    /// Draw a closed polygon provided in CCW order.
    void drawPolygon(const playrho::Length2* vertices, size_type vertexCount, const HG::Utils::Color& color);

    /// Draw a solid closed polygon provided in CCW order.
    void drawSolidPolygon(const playrho::Length2* vertices, size_type vertexCount, const HG::Utils::Color& color);

    /// Draw a circle.
    void drawCircle(const playrho::Length2& center, playrho::Length radius, const HG::Utils::Color& color);

    /// Draw a solid circle.
    void drawSolidCircle(const playrho::Length2& center, playrho::Length radius, const HG::Utils::Color& color);

    /// Draw a line segment.
    void drawSegment(const playrho::Length2& p1, const playrho::Length2& p2, const HG::Utils::Color& color);

    /// Draw point
    void drawPoint(const playrho::Length2& p, playrho::Length size, const HG::Utils::Color& color);

    /// Draws a string at the given screen coordinates.
    void drawString(int x, int y, const char* string, ...);

    /// Draws a string at the given world coordinates.
    void drawString(const playrho::Length2& p, const char* string, ...);

private:
    HG::Rendering::Base::Gizmos* m_gizmos;

    float m_z;
};
} // namespace HG::Physics::PlayRho
