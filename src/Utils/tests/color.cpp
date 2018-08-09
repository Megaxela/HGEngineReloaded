#include <gtest/gtest.h>
#include <Color.hpp>

TEST(Utils, ColorIdentity)
{
    for (int i = 0; i < 256; ++i)
    {
        HG::Utils::Color color = HG::Utils::Color::fromRGB(i, i, i, i);

        ASSERT_EQ(color.red(),   i);
        ASSERT_EQ(color.green(), i);
        ASSERT_EQ(color.blue(),  i);
        ASSERT_EQ(color.alpha(), i);
    }
}