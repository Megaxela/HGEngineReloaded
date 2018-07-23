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

UTILS_MODULE_NS::Color UTILS_MODULE_NS::Color::fromHex(const char* hex, std::size_t len)
{
    if (len == 0)
    {
        return UTILS_MODULE_NS::Color();
    }

    if (*hex == '#')
    {
        ++hex;
        --len;
    }

    if (len % 2 != 0)
    {
        return UTILS_MODULE_NS::Color();
    }
    
    uint32_t data = 0;

    auto first = true;
    uint8_t firstValue = 0;
    uint8_t secondValue = 0;

    while (len > 0)
    {
        if (first)
        {
            first = false;

            if ((*hex) >= '0' && (*hex) <= '9')
            {
                firstValue = static_cast<uint8_t>((*hex) - '0');
            }
            else if ((*hex) >= 'A' && (*hex) <= 'F')
            {
                firstValue = static_cast<uint8_t>((*hex) - 'A' + 10);
            }
            else if ((*hex) >= 'a' && (*hex) <= 'f')
            {
                firstValue = static_cast<uint8_t>((*hex) - 'a' + 10);
            }
            else
            {
                return UTILS_MODULE_NS::Color();
            }
        }
        else
        {
            first = true;

            if ((*hex) >= '0' && (*hex) <= '9')
            {
                secondValue = static_cast<uint8_t>((*hex) - '0');
            }
            else if ((*hex) >= 'A' && (*hex) <= 'F')
            {
                secondValue = static_cast<uint8_t>((*hex) - 'A' + 10);
            }
            else if ((*hex) >= 'a' && (*hex) <= 'f')
            {
                secondValue = static_cast<uint8_t>((*hex) - 'a' + 10);
            }
            else
            {
                return UTILS_MODULE_NS::Color();
            }

            data <<= 8;
            data |= firstValue * 16 + secondValue;
        }

        ++hex;
        --len;
    }

    return fromRaw(data);
}

UTILS_MODULE_NS::Color UTILS_MODULE_NS::Color::fromRaw(uint32_t data)
{
    Color c;

    c.m_r = ( data & 0xFFU      )        / 255.0f;
    c.m_g = ((data & 0xFF00U    ) >> 8 ) / 255.0f;
    c.m_b = ((data & 0xFF0000U  ) >> 16) / 255.0f;
    c.m_a = ((data & 0xFF000000U) >> 24) / 255.0f;

    return c;
}

UTILS_MODULE_NS::Color UTILS_MODULE_NS::Color::fromHex(const std::string& s)
{
    return fromHex(s.c_str(), s.size());
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
