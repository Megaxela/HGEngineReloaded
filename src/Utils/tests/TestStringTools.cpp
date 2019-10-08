// HG::Utils
#include <HG/Utils/StringTools.hpp>

// GTest
#include <gtest/gtest.h>

TEST(Utils, StringToolsHashZeroString)
{
    size_t hash = 0;
    ASSERT_NO_THROW(hash = HG::Utils::StringTools::hash(""));
}

TEST(Utils, StringToolsHashHappyPath)
{
    size_t hash1 = HG::Utils::StringTools::hash("Some Long String");
    size_t hash2 = HG::Utils::StringTools::hash("Some Long String");
    size_t hash3 = HG::Utils::StringTools::hash("Some Long String2");
    size_t hash4 = HG::Utils::StringTools::hash("Some Long Strinh");

    ASSERT_EQ(hash1, hash2);
    ASSERT_NE(hash1, hash3);
    ASSERT_NE(hash1, hash4);
    ASSERT_NE(hash3, hash4);
}

TEST(Utils, StringToolsSplitHappyPath)
{
    std::string someText = "some text to split on spaces";

    std::vector<std::string> result;
    HG::Utils::StringTools::split(someText, ' ', std::back_inserter(result));

    ASSERT_EQ(result.size(), 6);
    ASSERT_EQ(result, std::vector<std::string>({"some", "text", "to", "split", "on", "spaces"}));
}

TEST(Utils, StringToolsSplitTrailing)
{
    std::string someText = "|some|text|";

    std::vector<std::string> result;
    HG::Utils::StringTools::split(someText, '|', std::back_inserter(result));

    ASSERT_EQ(result.size(), 4);
    ASSERT_EQ(result, std::vector<std::string>({"", "some", "text", ""}));
}

TEST(Utils, StringToolsSplitSimple)
{
    std::string someText = "simple text for test";
    auto splitted        = HG::Utils::StringTools::split(someText, ' ');

    ASSERT_EQ(splitted, std::vector<std::string>({"simple", "text", "for", "test"}));
}

TEST(Utils, StringToolsSplitHappyPathWide)
{
    std::wstring someText = L"some text to split on spaces";

    std::vector<std::wstring> result;
    HG::Utils::StringTools::split(someText, ' ', std::back_inserter(result));

    ASSERT_EQ(result.size(), 6);
    ASSERT_EQ(result, std::vector<std::wstring>({L"some", L"text", L"to", L"split", L"on", L"spaces"}));
}

TEST(Utils, StringToolsSplitTrailingWide)
{
    std::wstring someText = L"|some|text|";

    std::vector<std::wstring> result;
    HG::Utils::StringTools::split(someText, '|', std::back_inserter(result));

    ASSERT_EQ(result.size(), 4);
    ASSERT_EQ(result, std::vector<std::wstring>({L"", L"some", L"text", L""}));
}

TEST(Utils, StringToolsSplitSimpleWide)
{
    std::wstring someText = L"simple text for test";
    auto splitted         = HG::Utils::StringTools::split(someText, L' ');

    ASSERT_EQ(splitted, std::vector<std::wstring>({L"simple", L"text", L"for", L"test"}));
}

TEST(Utils, StringToolsEndsWithHappyPath)
{
    std::string source      = "some_text_with_ending";
    std::string trueEnding  = "with_ending";
    std::string falseEnding = "without_ending";

    ASSERT_TRUE(HG::Utils::StringTools::endsWith(source, trueEnding));
    ASSERT_FALSE(HG::Utils::StringTools::endsWith(source, falseEnding));
}

TEST(Utils, StringToolsEndsWithSizeMismatch)
{
    std::string source = "little_string";
    std::string ending = "really_huge_ending";

    ASSERT_FALSE(HG::Utils::StringTools::endsWith(source, ending));
}

TEST(Utils, StringToolsEndsWithHappyPathWide)
{
    std::wstring source      = L"some_text_with_ending";
    std::wstring trueEnding  = L"with_ending";
    std::wstring falseEnding = L"without_ending";

    ASSERT_TRUE(HG::Utils::StringTools::endsWith(source, trueEnding));
    ASSERT_FALSE(HG::Utils::StringTools::endsWith(source, falseEnding));
}

TEST(Utils, StringToolsEndsWithSizeMismatchWide)
{
    std::wstring source = L"little_string";
    std::wstring ending = L"really_huge_ending";

    ASSERT_FALSE(HG::Utils::StringTools::endsWith(source, ending));
}

TEST(Utils, StringToolsSmartSplitHappyPath)
{
    std::string source = "SOME \"COOL TEXT\"";

    auto result = HG::Utils::StringTools::smartSplit(source, ' ');

    ASSERT_EQ(result, std::vector<std::string>({"SOME", "COOL TEXT"}));
}

TEST(Utils, StringToolsSmartSplitScreening)
{
    std::string source = R"(SOME \"COOL TEXT\")";

    auto result = HG::Utils::StringTools::smartSplit(source, ' ');
    ASSERT_EQ(result, std::vector<std::string>({"SOME", "\"COOL", "TEXT\""}));
}

TEST(Utils, StringToolsSmartSplitHappyPathWide)
{
    std::wstring source = L"SOME \"COOL TEXT\"";

    auto result = HG::Utils::StringTools::smartSplit(source, L' ');

    ASSERT_EQ(result, std::vector<std::wstring>({L"SOME", L"COOL TEXT"}));
}

TEST(Utils, StringToolsSmartSplitScreeningWide)
{
    std::wstring source = LR"(SOME \"COOL TEXT\")";

    auto result = HG::Utils::StringTools::smartSplit(source, ' ');
    ASSERT_EQ(result, std::vector<std::wstring>({L"SOME", L"\"COOL", L"TEXT\""}));
}

TEST(Utils, StringToolsJoinHappyPath)
{
    std::vector<std::string> some_vector = {"test_1", "test_2", "test_3"};

    auto result = HG::Utils::StringTools::join(some_vector, ", ");

    ASSERT_EQ(result, "test_1, test_2, test_3");
}

TEST(Utils, StringToolsJoinEmpty)
{
    std::vector<std::string> empty_vector;

    auto result = HG::Utils::StringTools::join(empty_vector, ", ");

    ASSERT_EQ(result, "");
}

TEST(Utils, StringToolsJoinHappyPathWide)
{
    std::vector<std::wstring> some_vector = {L"test_1", L"test_2", L"test_3"};

    auto result = HG::Utils::StringTools::join(some_vector, L", ");

    ASSERT_EQ(result, L"test_1, test_2, test_3");
}

TEST(Utils, StringToolsJoinEmptyWide)
{
    std::vector<std::wstring> empty_vector;

    auto result = HG::Utils::StringTools::join(empty_vector, L", ");

    ASSERT_EQ(result, L"");
}
