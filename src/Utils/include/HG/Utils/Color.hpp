#pragma once

// C++ STL
#include <cstdint>
#include <string>

// GLM
#include <glm/glm.hpp>

namespace HG::Utils
{
/**
 * @brief Class, that describes
 * color value.
 */
class Color
{
public:
    static Color Red;
    static Color Green;
    static Color Blue;
    static Color White;
    static Color Gray;
    static Color Black;
    static Color Transparent;

    /**
     * @brief Default constructor.
     */
    Color();

    /**
     * @param r Value, between 0.0 and 1.0, describing component value.
     * @param g Value, between 0.0 and 1.0, describing component value.
     * @param b Value, between 0.0 and 1.0, describing component value.
     * @param a Value, between 0.0 and 1.0, describing component value.
     */
    Color(float r, float g, float b, float a = 1.0f);

    /**
     * @brief Color creator from RGBA values.
     * @param r Red value. 0 - 255
     * @param g Green value. 0 - 255
     * @param b Blue value. 0 - 255
     * @param a Alpha value. 0 - 255
     * @return Created Color instance.
     */
    static Color fromRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    /**
     * @brief Color creator from hex string
     * ("#FFAAFFAA" / "FFAAFFAA" / "FFAAFF")
     * @param hex C string.
     * @param len C string len.
     * @return Created color. (White color on error)
     */
    static Color fromHex(const char* hex, std::size_t len);

    /**
     * @brief Color creator from raw RGBA data.
     * 0x04030201 (r = 1, g = 2, b = 3, a = 4)
     * @param data Data.
     * @return Color.
     */
    static Color fromRaw(uint32_t data);

    /**
     * @brief Color creator from hex string
     * ("#FFAAFFAA" / "FFAAFFAA" / "FFAAFF") (ARGB)/(RGB)
     * @param s String.
     * @return Created color. (White color on error)
     */
    static Color fromHex(const std::string& s);

    /**
     * @brief Method to get red channel.
     * @return Color red channel. 0 - 255
     */
    uint8_t red() const;

    /**
     * @brief Method to get green channel.
     * @return Color green channel. 0 - 255
     */
    uint8_t green() const;

    /**
     * @brief Method to get blue channel.
     * @return Color blue channel. 0 - 255
     */
    uint8_t blue() const;

    /**
     * @brief Method to get alpha channel.
     * @return Color alpha channel. 0 - 255
     */
    uint8_t alpha() const;

    /**
     * @brief Method to get red channel.
     * @return Color red channel. 0.0 - 1.0
     */
    float redF() const;

    /**
     * @brief Method to get green channel.
     * @return Color green channel. 0.0 - 1.0
     */
    float greenF() const;

    /**
     * @brief Method to get blue channel.
     * @return Color blue channel. 0.0 - 1.0
     */
    float blueF() const;

    /**
     * @brief Method to get alpha channel.
     * @return Color alpha channel. 0.0 - 1.0
     */
    float alphaF() const;

    /**
     * @brief Method for converting color to RGB vector.
     * @return GLM 3 component vector.
     */
    glm::vec3 toRGBVector() const;

    /**
     * @brief Method for converting color to RGBA vector.
     * @return GLM 4 component vector.
     */
    glm::vec4 toRGBAVector() const;

    /**
     * @brief Static method for getting more bright color.
     * @param color Color.
     * @param factor Factor of brightness.
     * @return Brighten copy of initial color.
     */
    static Color brighten(const Color& color, float factor);

    /**
     * @brief Method for checking equality of colors.
     */
    bool operator==(const Color& rhs) const;

private:
    float m_r;
    float m_g;
    float m_b;
    float m_a = 1;
};

std::ostream& operator<<(std::ostream& stream, HG::Utils::Color& color);
} // namespace HG::Utils