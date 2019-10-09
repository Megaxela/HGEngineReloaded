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

        ASSERT_EQ(color.red(), i);
        ASSERT_EQ(color.green(), i);
        ASSERT_EQ(color.blue(), i);
        ASSERT_EQ(color.alpha(), i);
    }
}