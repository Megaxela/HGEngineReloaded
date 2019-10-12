// C++ STL
#include <iostream>

// HG::Utils
#include <HG/Utils/Color.hpp>

namespace
{
template <typename T>
T clamp(const T& n, const T& lower, const T& upper)
{
    return std::max(lower, std::min(n, upper));
}
} // namespace

namespace HG::Utils
{
Color Color::Red(1.0f, 0.0f, 0.0f);
Color Color::Green(0.0f, 1.0f, 0.0f);
Color Color::Blue(0.0f, 0.0f, 1.0f);
Color Color::White(1.0f, 1.0f, 1.0f);
Color Color::Gray(0.8f, 0.8f, 0.8f);
Color Color::Black(0.0f, 0.0f, 0.0f);
Color Color::Transparent(0.0f, 0.0f, 0.0f, 0.0f);

Color::Color() : m_r(0.0f), m_g(0.0f), m_b(0.0f), m_a(1.0f)
{
}

Color::Color(float r, float g, float b, float a) :
    m_r(clamp(r, 0.0f, 1.0f)),
    m_g(clamp(g, 0.0f, 1.0f)),
    m_b(clamp(b, 0.0f, 1.0f)),
    m_a(clamp(a, 0.0f, 1.0f))
{
}

Color Color::fromRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
    return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

Color Color::fromHex(const char* hex, std::size_t len)
{
    if (len == 0)
    {
        return Color();
    }

    if (*hex == '#')
    {
        ++hex;
        --len;
    }

    if (len != 6 && len != 8)
    {
        return Color();
    }

    std::uint32_t data = 0;

    auto first               = true;
    std::uint8_t firstValue  = 0;
    std::uint8_t secondValue = 0;
    auto isRGB               = len == 6;

    while (len > 0)
    {
        if (first)
        {
            first = false;

            if ((*hex) >= '0' && (*hex) <= '9')
            {
                firstValue = static_cast<std::uint8_t>((*hex) - '0');
            }
            else if ((*hex) >= 'A' && (*hex) <= 'F')
            {
                firstValue = static_cast<std::uint8_t>((*hex) - 'A' + 10);
            }
            else if ((*hex) >= 'a' && (*hex) <= 'f')
            {
                firstValue = static_cast<std::uint8_t>((*hex) - 'a' + 10);
            }
            else
            {
                return Color();
            }
        }
        else
        {
            first = true;

            if ((*hex) >= '0' && (*hex) <= '9')
            {
                secondValue = static_cast<std::uint8_t>((*hex) - '0');
            }
            else if ((*hex) >= 'A' && (*hex) <= 'F')
            {
                secondValue = static_cast<std::uint8_t>((*hex) - 'A' + 10);
            }
            else if ((*hex) >= 'a' && (*hex) <= 'f')
            {
                secondValue = static_cast<std::uint8_t>((*hex) - 'a' + 10);
            }
            else
            {
                return Color();
            }

            data <<= 8;
            data |= firstValue * 16 + secondValue;
        }

        ++hex;
        --len;
    }

    std::uint8_t alpha = 0xFF;

    if (!isRGB)
    {
        alpha = static_cast<std::uint8_t>((data & 0xFF000000) >> 24);
    }

    data <<= 8;
    data |= alpha;

    return fromRaw(data);
}

Color Color::fromRaw(std::uint32_t data)
{
    Color c;

    c.m_a = (data & 0xFFU) / 255.0f;
    c.m_b = ((data & 0xFF00U) >> 8) / 255.0f;
    c.m_g = ((data & 0xFF0000U) >> 16) / 255.0f;
    c.m_r = ((data & 0xFF000000U) >> 24) / 255.0f;

    return c;
}

Color Color::fromHex(const std::string& s)
{
    return fromHex(s.c_str(), s.size());
}

std::uint8_t Color::red() const
{
    return (std::uint8_t)(m_r * 255);
}

std::uint8_t Color::green() const
{
    return (std::uint8_t)(m_g * 255);
}

std::uint8_t Color::blue() const
{
    return (std::uint8_t)(m_b * 255);
}

std::uint8_t Color::alpha() const
{
    return (std::uint8_t)(m_a * 255);
}

float Color::redF() const
{
    return m_r;
}

float Color::greenF() const
{
    return m_g;
}

float Color::blueF() const
{
    return m_b;
}

float Color::alphaF() const
{
    return m_a;
}

glm::vec3 Color::toRGBVector() const
{
    return glm::vec3(redF(), greenF(), blueF());
}

glm::vec4 Color::toRGBAVector() const
{
    return glm::vec4(redF(), greenF(), blueF(), alphaF());
}

Color Color::brighten(const Color& color, float factor)
{
    return Color(clamp(color.m_r * factor, 0.0f, 1.0f),
                 clamp(color.m_g * factor, 0.0f, 1.0f),
                 clamp(color.m_b * factor, 0.0f, 1.0f),
                 color.m_a);
}

bool Color::operator==(const Color& rhs) const
{
    return std::abs(rhs.m_r - m_r) < 0.01 && std::abs(rhs.m_g - m_g) < 0.01 && std::abs(rhs.m_b - m_b) < 0.01 &&
           std::abs(rhs.m_a - m_a) < 0.01;
}

std::ostream& operator<<(std::ostream& stream, Color& color)
{
    stream << "Color(" << int(color.red()) << ", " << int(color.green()) << ", " << int(color.blue()) << ", "
           << int(color.alpha()) << ")";

    return stream;
}
} // namespace HG::Utils
