#include <HG/Core/CachableResource.hpp>
#include <HG/Core/ResourceCache.hpp>
#include <gtest/gtest.h>

namespace
{
enum Action
{
    DerivedConstructed,
    DerivedDestructed,
    BaseConstructed,
    BaseDestructed
};

std::vector<int> sequence;
} // namespace

class SomeResource : public HG::Core::CachableResource<SomeResource>
{
public:
    SomeResource()
    {
        sequence.emplace_back(Action::DerivedConstructed);
    }

    ~SomeResource() override
    {
        sequence.emplace_back(Action::DerivedDestructed);
    }
};

TEST(Core, CachableResource)
{
    sequence.clear();
    std::vector<int> expected;

    {
        HG::Core::ResourceCache cache;

        // Constructed
        auto* obj1 = new (&cache) SomeResource;
        expected.emplace_back(Action::DerivedConstructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 1);

        // Constructed
        auto* obj2 = new (&cache) SomeResource;
        expected.emplace_back(Action::DerivedConstructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 2);

        // Destructed
        delete obj2;
        expected.emplace_back(Action::DerivedDestructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 1);

        // Constructed
        obj2 = new (&cache) SomeResource;
        expected.emplace_back(Action::DerivedConstructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 2);

        // Destructed
        delete obj2;
        expected.emplace_back(Action::DerivedDestructed);

        ASSERT_EQ(cache.getUsedResources<SomeResource>().size(), 1);

        // Checking
        ASSERT_EQ(sequence, expected);
    }

    int32_t z = 1;

    for (int i = 0; i < 64; ++i)
    {
        z <<= 1;
    }

    ASSERT_EQ(sequence, expected);
}

class SomeBase
{
public:
    SomeBase()
    {
        sequence.emplace_back(Action::BaseConstructed);
    }

    virtual ~SomeBase()
    {
        sequence.emplace_back(Action::BaseDestructed);
    }
};

class SomeDerived
    : public HG::Core::CachableResource<SomeDerived>
    , public SomeBase
{
public:
    SomeDerived()
    {
        sequence.emplace_back(Action::DerivedConstructed);
    }

    ~SomeDerived() override
    {
        sequence.emplace_back(Action::DerivedDestructed);
    }
};

TEST(Core, CachableResourceMultipleInheritance)
{
    {
        HG::Core::ResourceCache cache;

        for (int i = 0; i < 2; ++i)
        {
            new (&cache) SomeDerived;
        }
    }

    std::vector<int> expected = {Action::BaseConstructed,
                                 Action::DerivedConstructed,
                                 Action::BaseConstructed,
                                 Action::DerivedConstructed,
                                 Action::DerivedDestructed,
                                 Action::BaseDestructed,
                                 Action::DerivedDestructed,
                                 Action::BaseDestructed};
}