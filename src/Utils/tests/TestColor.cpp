// C++ STD
#include <unordered_map>

// HG::Utils
#include <HG/Utils/Color.hpp>

// GTest
#include <gtest/gtest.h>

// glm
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

TEST(Utils, ColorIdentity)
{
    for (int r = 0; r < 256; ++r)
    {
        for (int g = 0; g < 256; ++g)
        {
            for (int b = 0; b < 256; ++b)
            {
                HG::Utils::Color color = HG::Utils::Color::fromRGB(r, g, b, b);

                ASSERT_EQ(color.red(), r);
                ASSERT_EQ(color.green(), g);
                ASSERT_EQ(color.blue(), b);
                ASSERT_EQ(color.alpha(), b);
            }
        }
    }
}

struct ColorChannels
{
    int r;
    int g;
    int b;
    int a;
};

TEST(Utils, ColorFromHexWithSharp)
{
    std::unordered_map<std::string, ColorChannels> samples = {
        {"#AABBCCDD", {187, 204, 221, 170}},
        {"#00000000", {0, 0, 0, 0}},
        {"#FFFFFFFF", {255, 255, 255, 255}},
        {"#PPPPPPPP", {0, 0, 0, 255}},
        {"#2", {0, 0, 0, 255}},
    };

    for (const auto& [hexString, channels] : samples)
    {
        auto color = HG::Utils::Color::fromHex(hexString);

        ASSERT_EQ(color.red(), channels.r);
        ASSERT_EQ(color.green(), channels.g);
        ASSERT_EQ(color.blue(), channels.b);
        ASSERT_EQ(color.alpha(), channels.a);
    }
}

TEST(Utils, ColorFromHexWithoutSharp)
{
    std::unordered_map<std::string, ColorChannels> samples = {
        {"AABBCCDD", {187, 204, 221, 170}},
        {"00000000", {0, 0, 0, 0}},
        {"FFFFFFFF", {255, 255, 255, 255}},
        {"PPPPPPPP", {0, 0, 0, 255}},
        {"2", {0, 0, 0, 255}},
    };

    for (const auto& [hexString, channels] : samples)
    {
        auto color = HG::Utils::Color::fromHex(hexString);

        ASSERT_EQ(color.red(), channels.r);
        ASSERT_EQ(color.green(), channels.g);
        ASSERT_EQ(color.blue(), channels.b);
        ASSERT_EQ(color.alpha(), channels.a);
    }
}

TEST(Utils, ColorFromHexError)
{
    auto color = HG::Utils::Color::fromHex("");

    ASSERT_EQ(color.red(), 0);
    ASSERT_EQ(color.green(), 0);
    ASSERT_EQ(color.blue(), 0);
    ASSERT_EQ(color.alpha(), 255);
}

TEST(Utils, ColorFromHexErrorCString)
{
    auto color = HG::Utils::Color::fromHex("", 0);

    ASSERT_EQ(color.red(), 0);
    ASSERT_EQ(color.green(), 0);
    ASSERT_EQ(color.blue(), 0);
    ASSERT_EQ(color.alpha(), 255);
}

TEST(Utils, ColorFloatValues)
{
    auto color = HG::Utils::Color(0.0f, 0.3f, 0.8f, 1.0f);

    ASSERT_FLOAT_EQ(color.redF(), 0.0f);
    ASSERT_FLOAT_EQ(color.greenF(), 0.3f);
    ASSERT_FLOAT_EQ(color.blueF(), 0.8f);
    ASSERT_FLOAT_EQ(color.alphaF(), 1.0f);
}

TEST(Utils, ColorToVector)
{
    auto color = HG::Utils::Color(0.0f, 0.3f, 0.8f, 1.0f);

    ASSERT_EQ(color.toRGBVector(), glm::vec3(0.0f, 0.3f, 0.8f));
    ASSERT_EQ(color.toRGBAVector(), glm::vec4(0.0f, 0.3f, 0.8f, 1.0f));
}

TEST(Utils, ColorBrighten)
{
    auto color = HG::Utils::Color(0.2f, 0.4f, 0.8f, 0.8f);

    auto new_color = HG::Utils::Color::brighten(color, 2);

    ASSERT_FLOAT_EQ(new_color.redF(), 0.4f);
    ASSERT_FLOAT_EQ(new_color.greenF(), 0.8f);
    ASSERT_FLOAT_EQ(new_color.blueF(), 1.0f);  // clamped
    ASSERT_FLOAT_EQ(new_color.alphaF(), 0.8f); // unchanged
}

TEST(Utils, ColorToString)
{
    auto color = HG::Utils::Color();

    std::stringstream ss;
    ss << color;

    ASSERT_FALSE(ss.str().empty());
}
