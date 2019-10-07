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

HG::Utils::Color HG::Utils::Color::Red(1.0f, 0.0f, 0.0f);
HG::Utils::Color HG::Utils::Color::Green(0.0f, 1.0f, 0.0f);
HG::Utils::Color HG::Utils::Color::Blue(0.0f, 0.0f, 1.0f);
HG::Utils::Color HG::Utils::Color::White(1.0f, 1.0f, 1.0f);
HG::Utils::Color HG::Utils::Color::Gray(0.8f, 0.8f, 0.8f);
HG::Utils::Color HG::Utils::Color::Black(0.0f, 0.0f, 0.0f);
HG::Utils::Color HG::Utils::Color::Transparent(0.0f, 0.0f, 0.0f, 0.0f);

HG::Utils::Color::Color() : m_r(0.0f), m_g(0.0f), m_b(0.0f), m_a(1.0f)
{
}

HG::Utils::Color::Color(float r, float g, float b, float a) :
    m_r(clamp(r, 0.0f, 1.0f)),
    m_g(clamp(g, 0.0f, 1.0f)),
    m_b(clamp(b, 0.0f, 1.0f)),
    m_a(clamp(a, 0.0f, 1.0f))
{
}

HG::Utils::Color HG::Utils::Color::fromRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

HG::Utils::Color HG::Utils::Color::fromHex(const char* hex, std::size_t len)
{
    if (len == 0)
    {
        return HG::Utils::Color();
    }

    if (*hex == '#')
    {
        ++hex;
        --len;
    }

    if (len != 6 && len != 8)
    {
        return HG::Utils::Color();
    }

    uint32_t data = 0;

    auto first          = true;
    uint8_t firstValue  = 0;
    uint8_t secondValue = 0;
    auto isRGB          = len == 6;

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
                return HG::Utils::Color();
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
                return HG::Utils::Color();
            }

            data <<= 8;
            data |= firstValue * 16 + secondValue;
        }

        ++hex;
        --len;
    }

    uint8_t alpha = 0xFF;

    if (!isRGB)
    {
        alpha = static_cast<uint8_t>((data & 0xFF000000) >> 24);
    }

    data <<= 8;
    data |= alpha;

    return fromRaw(data);
}

HG::Utils::Color HG::Utils::Color::fromRaw(uint32_t data)
{
    Color c;

    c.m_a = (data & 0xFFU) / 255.0f;
    c.m_b = ((data & 0xFF00U) >> 8) / 255.0f;
    c.m_g = ((data & 0xFF0000U) >> 16) / 255.0f;
    c.m_r = ((data & 0xFF000000U) >> 24) / 255.0f;

    return c;
}

HG::Utils::Color HG::Utils::Color::fromHex(const std::string& s)
{
    return fromHex(s.c_str(), s.size());
}

uint8_t HG::Utils::Color::red() const
{
    return (uint8_t)(m_r * 255);
}

uint8_t HG::Utils::Color::green() const
{
    return (uint8_t)(m_g * 255);
}

uint8_t HG::Utils::Color::blue() const
{
    return (uint8_t)(m_b * 255);
}

uint8_t HG::Utils::Color::alpha() const
{
    return (uint8_t)(m_a * 255);
}

float HG::Utils::Color::redF() const
{
    return m_r;
}

float HG::Utils::Color::greenF() const
{
    return m_g;
}

float HG::Utils::Color::blueF() const
{
    return m_b;
}

float HG::Utils::Color::alphaF() const
{
    return m_a;
}

glm::vec3 HG::Utils::Color::toRGBVector() const
{
    return glm::vec3(redF(), greenF(), blueF());
}

glm::vec4 HG::Utils::Color::toRGBAVector() const
{
    return glm::vec4(redF(), greenF(), blueF(), alphaF());
}

HG::Utils::Color HG::Utils::Color::brighten(const HG::Utils::Color& color, float factor)
{
    return Color(color.m_r * factor, color.m_g * factor, color.m_b * factor, color.m_a);
}

bool HG::Utils::Color::operator==(const HG::Utils::Color& rhs) const
{
    return std::abs(rhs.m_r - m_r) < 0.01 && std::abs(rhs.m_g - m_g) < 0.01 && std::abs(rhs.m_b - m_b) < 0.01 &&
           std::abs(rhs.m_a - m_a) < 0.01;
}

std::ostream& HG::Utils::operator<<(std::ostream& stream, HG::Utils::Color& color)
{
    stream << "Color(" << int(color.red()) << ", " << int(color.green()) << ", " << int(color.blue()) << ", "
           << int(color.alpha()) << ")";

    return stream;
}
