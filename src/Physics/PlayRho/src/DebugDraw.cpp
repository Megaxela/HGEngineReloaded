
#include <DebugDraw.hpp>

PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::DebugDraw(RENDERING_BASE_MODULE_NS::Gizmos *gizmos) :
    m_gizmos(gizmos),
    m_z(1.0f)
{

}

void PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::drawPolygon(const playrho::Length2 *vertices,
                                                       PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::size_type vertexCount,
                                                       const UTILS_MODULE_NS::Color &color)
{
    RENDERING_BASE_MODULE_NS::Gizmos::Line line;
    line.reserve(vertexCount);

    for (size_type i = 0; i < vertexCount; ++i)
    {
        line.push_back({vertices[i][0], vertices[i][1], m_z});
    }

    line.push_back(line[0]);

    m_gizmos->multiLine(line, color);
}

void PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::drawSolidPolygon(const playrho::Length2 *vertices,
                                                            PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::size_type vertexCount,
                                                            const UTILS_MODULE_NS::Color &color)
{
    // todo: Fix polygon solidity
    RENDERING_BASE_MODULE_NS::Gizmos::Line line;
    line.reserve(vertexCount);

    for (size_type i = 0; i < vertexCount; ++i)
    {
        line.push_back({vertices[i][0], vertices[i][1], m_z});
    }

    line.push_back(line[0]);

    m_gizmos->multiLine(line, color);
}

void PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::drawCircle(const playrho::Length2 &center,
                                                      playrho::Length radius,
                                                      const UTILS_MODULE_NS::Color &color)
{
    constexpr size_type k_segments = 16;

    constexpr float k_increment = 2.0f * playrho::Pi / k_segments;
    float theta = 0.0f;

    RENDERING_BASE_MODULE_NS::Gizmos::Line line;
    line.reserve(k_segments);

    for (int32_t i = 0; i < k_segments + 1; ++i)
    {
        playrho::Vec2 v = center + radius * playrho::Vec2(std::cos(theta), std::sin(theta));

        line.push_back(glm::fvec3(v[0], v[1], m_z));

        theta += k_increment;
    }

    m_gizmos->multiLine(line, color);
}

void PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::drawSolidCircle(const playrho::Length2 &center,
                                                           playrho::Length radius,
                                                           const UTILS_MODULE_NS::Color &color)
{
    // todo: Fix circle solidity
    constexpr size_type k_segments = 16;

    constexpr float k_increment = 2.0f * playrho::Pi / k_segments;
    float theta = 0.0f;

    RENDERING_BASE_MODULE_NS::Gizmos::Line line;
    line.reserve(k_segments);

    for (int32_t i = 0; i < k_segments + 1; ++i)
    {
        playrho::Vec2 v = center + radius * playrho::Vec2(std::cos(theta), std::sin(theta));

        line.push_back(glm::fvec3(v[0], v[1], m_z));

        theta += k_increment;
    }

    m_gizmos->multiLine(line, color);
}

void PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::drawSegment(const playrho::Length2 &p1,
                                                       const playrho::Length2 &p2,
                                                       const UTILS_MODULE_NS::Color &color)
{
    m_gizmos->line(
        glm::vec3(p1[0], p1[1], m_z),
        glm::vec3(p2[0], p2[1], m_z),
        color
    );
}

void PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::drawPoint(const playrho::Length2 &p,
                                                     playrho::Length size,
                                                     const UTILS_MODULE_NS::Color &color)
{
    drawSegment(p, p + playrho::Length2(0.1, 0.1), color);
}

void PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::drawString(int x, int y, const char *string, ...)
{
    // No implementation
}

void PLAYRHO_PHYSICS_MODULE_NS::DebugDraw::drawString(const playrho::Length2 &p, const char *string, ...)
{
    // No implementation
}
