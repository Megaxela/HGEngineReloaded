// HG::Rendering::Base
#include <HG/Rendering/Base/Gizmos.hpp>

// GLM
#include <glm/gtx/compatibility.hpp>

HG::Utils::Color HG::Rendering::Base::Gizmos::defaultColor = HG::Utils::Color::fromRGB(0, 0, 180, 180);

void HG::Rendering::Base::Gizmos::clear()
{
    m_linesBuffer.clear();
    m_sphereBuffer.clear();
    m_hexahedronBuffer.clear();
}

void HG::Rendering::Base::Gizmos::line(const glm::vec3& from, const glm::vec3& to, HG::Utils::Color color)
{
    line(from, color, to, color);
}

void HG::Rendering::Base::Gizmos::line(const glm::vec3& from,
                                       const HG::Utils::Color& fromColor,
                                       const glm::vec3& to,
                                       const HG::Utils::Color& toColor)
{
    m_linesBuffer.emplace_back(from, fromColor, to, toColor);
}

void HG::Rendering::Base::Gizmos::multiLine(const HG::Rendering::Base::Gizmos::Line& lineData, HG::Utils::Color color)
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

void HG::Rendering::Base::Gizmos::line(const HG::Rendering::Base::Gizmos::LineSegment& lineSegment,
                                       HG::Utils::Color color)
{
    line(lineSegment.first, lineSegment.second, color);
}

void HG::Rendering::Base::Gizmos::circle(const glm::vec3& position, float radius, const HG::Utils::Color& color)
{
    circle(position, {radius * 2, radius * 2}, color);
}

void HG::Rendering::Base::Gizmos::circle(const glm::vec3& position, glm::vec2 size, const HG::Utils::Color& color)
{
    HG::Core::Transform t;
    t.setGlobalPosition(position);

    circle(t, size, 32, color);
}

void HG::Rendering::Base::Gizmos::circle(const HG::Core::Transform& transform,
                                         glm::vec2 size,
                                         uint32_t parts,
                                         const HG::Utils::Color& color)
{
    // todo: Apply transform rotation and scale
    // Factor
    auto radiuses = size / 2.0f;

    /*
     *                        ab
     * r = -----------------------------------------
     *      sqrt(a^2 * sin^2(th) + b^2 * cos^2(th))
     */
    auto radius = (radiuses.x * radiuses.y) / glm::sqrt(radiuses.x * radiuses.x * glm::sin(0) * glm::sin(0) +
                                                        radiuses.y * radiuses.y * glm::cos(0) * glm::cos(0));

    glm::vec3 offset{glm::cos(0) * radius, glm::sin(0) * radius, 0};

    auto center = transform.globalPosition();

    auto firstPoint = center + offset;

    auto lastPoint = firstPoint;

    for (uint32_t i = 1; i < parts; ++i)
    {
        auto angleRadians = (2 * glm::pi<float>()) / parts * i;

        // Sick optimizations (no)
        auto angleSin = glm::sin(angleRadians);
        auto angleCos = glm::cos(angleRadians);

        radius = (radiuses.x * radiuses.y) / glm::sqrt(radiuses.x * radiuses.x * angleSin * angleSin +
                                                       radiuses.y * radiuses.y * angleCos * angleCos);

        auto newPoint = center + glm::vec3{angleCos * radius, angleSin * radius, 0};

        line(lastPoint, newPoint, color);

        lastPoint = newPoint;
    }

    line(lastPoint, firstPoint, color);
}

void HG::Rendering::Base::Gizmos::sphere(const HG::Core::Transform& transform, float radius, HG::Utils::Color color)
{
    sphere(transform, radius, 15, color);
}

void HG::Rendering::Base::Gizmos::sphere(const HG::Core::Transform& transform,
                                         float radius,
                                         int dividings,
                                         HG::Utils::Color color)
{
    m_sphereBuffer.emplace_back(transform, dividings, radius, color);
}

void HG::Rendering::Base::Gizmos::hexahedron(const HG::Core::Transform& transform,
                                             float sideSize,
                                             HG::Utils::Color color)
{
    m_hexahedronBuffer.emplace_back(transform, sideSize, color);
}
