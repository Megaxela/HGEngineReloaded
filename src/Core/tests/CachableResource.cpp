#include <gtest/gtest.h>
#include <HG/Core/CachableResource.hpp>
#include <HG/Core/ResourceCache.hpp>

namespace {
    enum Action
    {
        Constructed,
        Destructed
    };

    std::vector<int> sequence;
}

class SomeResource : public HG::Core::CachableResource<SomeResource>
{
public:
    SomeResource()
    {
        sequence.emplace_back(Action::Constructed);
    }

    ~SomeResource() override
    {
        sequence.emplace_back(Action::Destructed);
    }
};

TEST(Core, CachableResource)
{
    std::vector<int> expected;

    {
        HG::Core::ResourceCache cache;

        // Constructed
        auto* obj1 = new (&cache) SomeResource;
        expected.emplace_back(Action::Constructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 1);

        // Constructed
        auto* obj2 = new (&cache) SomeResource;
        expected.emplace_back(Action::Constructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 2);

        // Destructed
        delete obj2;
        expected.emplace_back(Action::Destructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 1);

        // Constructed
        obj2 = new (&cache) SomeResource;
        expected.emplace_back(Action::Constructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 2);

        // Destructed
        delete obj2;
        expected.emplace_back(Action::Destructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 1);

        // Checking
        ASSERT_EQ(sequence, expected);

        expected.emplace_back(Action::Destructed);
    }

    ASSERT_EQ(sequence, expected);
}