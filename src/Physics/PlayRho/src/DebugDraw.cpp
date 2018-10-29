
// HG::Rendering::Base
#include <HG/Rendering/Base/Gizmos.hpp>

// HG::Physics::PlayRho
#include <DebugDraw.hpp>

HG::Physics::PlayRho::DebugDraw::DebugDraw(HG::Rendering::Base::Gizmos *gizmos) :
    m_gizmos(gizmos),
    m_z(1.0f)
{

}

void HG::Physics::PlayRho::DebugDraw::drawPolygon(const playrho::Length2 *vertices,
                                                       HG::Physics::PlayRho::DebugDraw::size_type vertexCount,
                                                       const HG::Utils::Color &color)
{
    HG::Rendering::Base::Gizmos::Line line;
    line.reserve(vertexCount);

    for (size_type i = 0; i < vertexCount; ++i)
    {
        line.push_back({vertices[i][0], vertices[i][1], m_z});
    }

    line.push_back(line[0]);

    m_gizmos->multiLine(line, color);
}

void HG::Physics::PlayRho::DebugDraw::drawSolidPolygon(const playrho::Length2 *vertices,
                                                            HG::Physics::PlayRho::DebugDraw::size_type vertexCount,
                                                            const HG::Utils::Color &color)
{
    // todo: Fix polygon solidity
    HG::Rendering::Base::Gizmos::Line line;
    line.reserve(vertexCount);

    for (size_type i = 0; i < vertexCount; ++i)
    {
        line.push_back({vertices[i][0], vertices[i][1], m_z});
    }

    line.push_back(line[0]);

    m_gizmos->multiLine(line, color);
}

void HG::Physics::PlayRho::DebugDraw::drawCircle(const playrho::Length2 &center,
                                                      playrho::Length radius,
                                                      const HG::Utils::Color &color)
{
    constexpr size_type k_segments = 16;

    constexpr float k_increment = 2.0f * playrho::Pi / k_segments;
    float theta = 0.0f;

    HG::Rendering::Base::Gizmos::Line line;
    line.reserve(k_segments);

    for (size_type i = 0; i < k_segments + 1; ++i)
    {
        playrho::Vec2 v = center + radius * playrho::Vec2(std::cos(theta), std::sin(theta));

        line.push_back(glm::fvec3(v[0], v[1], m_z));

        theta += k_increment;
    }

    m_gizmos->multiLine(line, color);
}

void HG::Physics::PlayRho::DebugDraw::drawSolidCircle(const playrho::Length2 &center,
                                                           playrho::Length radius,
                                                           const HG::Utils::Color &color)
{
    // todo: Fix circle solidity
    constexpr size_type k_segments = 16;

    constexpr float k_increment = 2.0f * playrho::Pi / k_segments;
    float theta = 0.0f;

    HG::Rendering::Base::Gizmos::Line line;
    line.reserve(k_segments);

    for (size_type i = 0; i < k_segments + 1; ++i)
    {
        playrho::Vec2 v = center + radius * playrho::Vec2(std::cos(theta), std::sin(theta));

        line.push_back(glm::fvec3(v[0], v[1], m_z));

        theta += k_increment;
    }

    m_gizmos->multiLine(line, color);
}

void HG::Physics::PlayRho::DebugDraw::drawSegment(const playrho::Length2 &p1,
                                                       const playrho::Length2 &p2,
                                                       const HG::Utils::Color &color)
{
    m_gizmos->line(
        glm::vec3(p1[0], p1[1], m_z),
        glm::vec3(p2[0], p2[1], m_z),
        color
    );
}

void HG::Physics::PlayRho::DebugDraw::drawPoint(const playrho::Length2 &p,
                                                     playrho::Length size,
                                                     const HG::Utils::Color &color)
{
    (void) size;
    drawSegment(p, p + playrho::Length2(0.1, 0.1), color);
}

void HG::Physics::PlayRho::DebugDraw::drawString(int x, int y, const char *string, ...)
{
    // todo: Remove this method.
    (void) x;
    (void) y;
    (void) string;
    // No implementation
}

void HG::Physics::PlayRho::DebugDraw::drawString(const playrho::Length2 &p, const char *string, ...)
{
    // todo: Remove this method.
    (void) p;
    (void) string;
    // No implementation
}
