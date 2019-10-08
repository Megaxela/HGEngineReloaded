// HG::Utils
#include <HG/Utils/Rect.hpp>

// GTest
#include <gtest/gtest.h>

TEST(Utils, RectDefaultConstructor)
{
    auto rect = HG::Utils::Rect();

    ASSERT_EQ(rect.h, 0);
    ASSERT_EQ(rect.w, 0);
    ASSERT_EQ(rect.x, 0);
    ASSERT_EQ(rect.y, 0);
}

TEST(Utils, RectInitConstructor)
{
    auto rect = HG::Utils::Rect(1, 2, 3, 4);

    ASSERT_EQ(rect.x, 1);
    ASSERT_EQ(rect.y, 2);
    ASSERT_EQ(rect.w, 3);
    ASSERT_EQ(rect.h, 4);
}

TEST(Utils, RectComparing)
{
    auto rect1 = HG::Utils::Rect(1, 2, 3, 4);
    auto rect2 = HG::Utils::Rect(1, 2, 3, 4);
    auto rect3 = HG::Utils::Rect(2, 3, 4, 5);

    ASSERT_EQ(rect1, rect2);
    ASSERT_NE(rect1, rect3);
}

TEST(Utils, RectToString)
{
    std::stringstream ss;
    ss << HG::Utils::Rect(1, 2, 3, 4);

    ASSERT_FALSE(ss.str().empty());
}