#include "Color.hpp"

namespace
{
    template <typename T>
    T clamp(const T& n, const T& lower, const T& upper)
    {
        return std::max(lower, std::min(n, upper));
    }
}

UTILS_MODULE_NS::Color::Color() :
    m_r(0.0f),
    m_g(0.0f),
    m_b(0.0f),
    m_a(1.0f)
{

}

UTILS_MODULE_NS::Color::Color(float r, float g, float b, float a) :
    m_r(clamp(r, 0.0f, 1.0f)),
    m_g(clamp(g, 0.0f, 1.0f)),
    m_b(clamp(b, 0.0f, 1.0f)),
    m_a(clamp(a, 0.0f, 1.0f))
{

}

UTILS_MODULE_NS::Color UTILS_MODULE_NS::Color::fromRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return Color(r / 255.0f,
                 g / 255.0f,
                 b / 255.0f,
                 a / 255.0f);
}

uint8_t UTILS_MODULE_NS::Color::red() const
{
    return (uint8_t) (m_r * 255);
}

uint8_t UTILS_MODULE_NS::Color::green() const
{
    return (uint8_t) (m_g * 255);
}

uint8_t UTILS_MODULE_NS::Color::blue() const
{
    return (uint8_t) (m_b * 255);
}

uint8_t UTILS_MODULE_NS::Color::alpha() const
{
    return (uint8_t) (m_a * 255);
}

float UTILS_MODULE_NS::Color::redF() const
{
    return m_r;
}

float UTILS_MODULE_NS::Color::greenF() const
{
    return m_g;
}

float UTILS_MODULE_NS::Color::blueF() const
{
    return m_b;
}

float UTILS_MODULE_NS::Color::alphaF() const
{
    return m_a;
}

glm::vec3 UTILS_MODULE_NS::Color::toRGBVector() const
{
    return glm::vec3(redF(), greenF(), blueF());
}

glm::vec4 UTILS_MODULE_NS::Color::toRGBAVector() const
{
    return glm::vec4(redF(), greenF(), blueF(), alphaF());
}

UTILS_MODULE_NS::Color UTILS_MODULE_NS::Color::brighten(const UTILS_MODULE_NS::Color& color, float factor)
{
    return Color(color.m_r * factor, color.m_g * factor, color.m_b * factor, color.m_a);
}
