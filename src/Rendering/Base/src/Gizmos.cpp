
#include <Gizmos.hpp>
#include <glm/gtx/compatibility.hpp>

UTILS_MODULE_NS::Color RENDERING_BASE_MODULE_NS::Gizmos::defaultColor 
    = UTILS_MODULE_NS::Color::fromRGB(0, 0, 180, 180);

void RENDERING_BASE_MODULE_NS::Gizmos::clear()
{
    m_linesBuffer.clear();
    m_sphereBuffer.clear();
    m_hexahedronBuffer.clear();
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

void RENDERING_BASE_MODULE_NS::Gizmos::circle(const glm::vec3 &position,
                                              float radius,
                                              const HG::Utils::Color &color)
{
    circle(position, {radius * 2, radius * 2}, color);
}

void HG::Rendering::Base::Gizmos::circle(const glm::vec3 &position, glm::vec2 size, const HG::Utils::Color &color)
{
    CORE_MODULE_NS::Transform t;
    t.setGlobalPosition(position);

    circle(t, size, 32, color);
}

void RENDERING_BASE_MODULE_NS::Gizmos::circle(const CORE_MODULE_NS::Transform &transform,
                                              glm::vec2 size,
                                              uint32_t parts,
                                              const UTILS_MODULE_NS::Color &color)
{
    // todo: Apply transform rotation and scale
    // Factor
    auto radiuses = size / 2.0f;

    /*
     *                        ab
     * r = -----------------------------------------
     *      sqrt(a^2 * sin^2(th) + b^2 * cos^2(th))
     */
    auto radius = (radiuses.x * radiuses.y) /
        glm::sqrt(radiuses.x * radiuses.x * glm::sin(0) * glm::sin(0) +
                  radiuses.y * radiuses.y * glm::cos(0) * glm::cos(0));

    glm::vec3 offset{
        glm::cos(0) * radius,
        glm::sin(0) * radius,
        0
    };

    auto center = transform.globalPosition();

    auto firstPoint = center + offset;

    auto lastPoint = firstPoint;

    for (uint32_t i = 1; i < parts; ++i)
    {
        auto angleRadians = (2 * glm::pi<float>()) / parts * i;

        // Sick optimizations (no)
        auto angleSin = glm::sin(angleRadians);
        auto angleCos = glm::cos(angleRadians);

        radius = (radiuses.x * radiuses.y) /
                 glm::sqrt(radiuses.x * radiuses.x * angleSin * angleSin +
                           radiuses.y * radiuses.y * angleCos * angleCos);

        auto newPoint = center + glm::vec3{
            angleCos * radius,
            angleSin * radius,
            0
        };

        line(lastPoint, newPoint, color);

        lastPoint = newPoint;
    }

    line(lastPoint, firstPoint, color);
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
