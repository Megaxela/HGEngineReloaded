#include "Gizmos.hpp"

void RENDERING_BASE_MODULE_NS::Gizmos::clear()
{

}

void RENDERING_BASE_MODULE_NS::Gizmos::line(const glm::vec3 &from, 
                                            const glm::vec3 &to, 
                                            UTILS_MODULE_NS::Color color)
{
    line(from, color, to, color);
}

void RENDERING_BASE_MODULE_NS::Gizmos::line(const glm::vec3 &from,
                                            const UTILS_MODULE_NS::Color &fromColor,
                                            const glm::vec3 &to,
                                            const UTILS_MODULE_NS::Color &toColor)
{
    m_linesBuffer.emplace_back(
        from,
        fromColor,
        to,
        toColor
    );
}

void RENDERING_BASE_MODULE_NS::Gizmos::multiLine(const RENDERING_BASE_MODULE_NS::Gizmos::Line &lineData,
                                                 UTILS_MODULE_NS::Color color)
{
    if (lineData.size() <= 1)
    {
        return;
    }

    glm::vec3 prev = lineData[0];

    for (Line::size_type i = 1; i < lineData.size(); ++i)
    {
        line(prev, lineData[i], color);

        prev = lineData[i];
    }
}

void RENDERING_BASE_MODULE_NS::Gizmos::line(const RENDERING_BASE_MODULE_NS::Gizmos::LineSegment &lineSegment,
                                            UTILS_MODULE_NS::Color color)
{
    line(lineSegment.first, lineSegment.second, color);
}

void RENDERING_BASE_MODULE_NS::Gizmos::sphere(const CORE_MODULE_NS::Transform &transform,
                                              float radius,
                                              UTILS_MODULE_NS::Color color)
{
    sphere(transform, radius, 15, color);
}

void RENDERING_BASE_MODULE_NS::Gizmos::sphere(const CORE_MODULE_NS::Transform &transform,
                                              float radius,
                                              int dividings,
                                              UTILS_MODULE_NS::Color color)
{
    m_sphereBuffer.emplace_back(
        transform,
        dividings,
        radius,
        color
    );
}

void RENDERING_BASE_MODULE_NS::Gizmos::hexahedron(const CORE_MODULE_NS::Transform &transform,
                                                  float sideSize,
                                                  UTILS_MODULE_NS::Color color)
{
    m_hexahedronBuffer.emplace_back(
        transform,
        sideSize,
        color
    );
}
