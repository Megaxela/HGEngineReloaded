#include <gtest/gtest.h>
#include <Color.hpp>

TEST(Utils, ColorIdentity)
{
    for (int i = 0; i < 256; ++i)
    {
        UTILS_MODULE_NS::Color color = UTILS_MODULE_NS::Color::fromRGB(i, i, i, i);

        ASSERT_EQ(color.red(),   i);
        ASSERT_EQ(color.green(), i);
        ASSERT_EQ(color.blue(),  i);
        ASSERT_EQ(color.alpha(), i);
    }
}