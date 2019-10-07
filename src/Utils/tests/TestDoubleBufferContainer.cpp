#include <HG/Utils/DoubleBufferContainer.hpp>
#include <gtest/gtest.h>

TEST(Utils, DoubleBufferContainerConstructorsDefault)
{
    HG::Utils::DoubleBufferContainer<std::size_t> data;

    ASSERT_EQ(data.size(), 0);
    ASSERT_EQ(data.added().size(), 0);
    ASSERT_EQ(data.current().size(), 0);
    ASSERT_EQ(data.removable().size(), 0);
}

TEST(Utils, DoubleBufferContainerConstructorsCopy)
{
    HG::Utils::DoubleBufferContainer<std::size_t> data;

    data.add(1);
    data.add(2);
    data.add(3);

    data.merge();

    data.add(4);
    data.add(5);
    data.remove(3);

    HG::Utils::DoubleBufferContainer<std::size_t> copied(data);

    ASSERT_EQ(copied.size(), 3);
    ASSERT_EQ(copied.current().size(), copied.size());
    ASSERT_EQ(copied.removable().size(), 1);
    ASSERT_EQ(copied.added().size(), 2);
}

static std::size_t copied      = 0;
static std::size_t moved       = 0;
static std::size_t constructed = 0;

static std::size_t counter = 0;

class SampleData
{
public:
    SampleData() : data(++counter)
    {
        ++constructed;
    }

    SampleData(const SampleData& copy) : data(copy.data)
    {
        ++copied;
    }

    SampleData(SampleData&& move) noexcept : data(move.data)
    {
        ++moved;
    }

    SampleData& operator=(const SampleData& rhs)
    {
        ++copied;
        data = rhs.data;

        return (*this);
    }

    SampleData& operator=(SampleData&& rhs) noexcept
    {
        ++moved;
        data = rhs.data;

        return (*this);
    }

    bool operator==(const SampleData& rhs) const
    {
        return data == rhs.data;
    }

    std::size_t data;
};

namespace std
{
template <>
struct hash<SampleData>
{
    std::size_t operator()(const SampleData& d) const
    {
        return d.data;
    }
};

} // namespace std

TEST(Utils, DoubleBufferContainerConstructorsMove)
{
    HG::Utils::DoubleBufferContainer<SampleData> data;

    data.add(SampleData());
    data.add(SampleData());
    data.add(SampleData());

    data.merge();

    data.add(SampleData());
    data.add(SampleData());
    data.remove(SampleData());

    ASSERT_EQ(data.size(), 3);
    ASSERT_EQ(data.current().size(), data.size());
    ASSERT_EQ(data.removable().size(), 1);
    ASSERT_EQ(data.added().size(), 2);

    ASSERT_EQ(copied, 0);
    ASSERT_EQ(moved, 15); // todo: Too many I guess, but it's ok for now
    ASSERT_EQ(constructed, 6);

    copied      = 0;
    moved       = 0;
    constructed = 0;

    auto movedData = std::move(data);

    ASSERT_EQ(movedData.size(), 3);
    ASSERT_EQ(movedData.current().size(), movedData.size());
    ASSERT_EQ(movedData.removable().size(), 1);
    ASSERT_EQ(movedData.added().size(), 2);

    ASSERT_EQ(copied, 0);
    ASSERT_EQ(moved, 0);
    ASSERT_EQ(constructed, 0);
}

TEST(Utils, DoubleBufferContainerConstructorsModifiersAndGetters)
{
    HG::Utils::DoubleBufferContainer<std::size_t> data;

    ASSERT_EQ(data.size(), data.current().size());
    ASSERT_EQ(data.current().size(), 0);
    ASSERT_EQ(data.removable().size(), 0);
    ASSERT_EQ(data.added().size(), 0);

    data.add(1);
    data.add(1);
    data.add(1);

    data.add(2);
    data.add(2);

    data.add(3);

    ASSERT_EQ(data.current().size(), 0);
    ASSERT_EQ(data.removable().size(), 0);
    ASSERT_EQ(data.added().size(), 3);

    data.remove(4);

    ASSERT_EQ(data.current().size(), 0);
    ASSERT_EQ(data.removable().size(), 1);
    ASSERT_EQ(data.added().size(), 3);

    data.remove(1);

    ASSERT_EQ(data.isRemoving(1), true);

    ASSERT_EQ(data.current().size(), 0);
    ASSERT_EQ(data.removable().size(), 2);
    ASSERT_EQ(data.added().size(), 3);

    data.merge();

    ASSERT_EQ(data.current().size(), 2);
    ASSERT_EQ(data.removable().size(), 0);
    ASSERT_EQ(data.added().size(), 0);

    data.remove(2);
    data.add(4);

    ASSERT_EQ(data.current().size(), 2);
    ASSERT_EQ(data.removable().size(), 1);
    ASSERT_EQ(data.added().size(), 1);

    data.clearState();

    ASSERT_EQ(data.current().size(), 2);
    ASSERT_EQ(data.removable().size(), 0);
    ASSERT_EQ(data.added().size(), 0);

    ASSERT_EQ(data[0], 2);
    ASSERT_EQ(data[1], 3);

    std::vector<std::size_t> expected = {2, 3};
    std::vector<std::size_t> actual(data.begin(), data.end());

    ASSERT_EQ(expected, actual);

    data.clear();

    ASSERT_EQ(data.current().size(), 0);
    ASSERT_EQ(data.removable().size(), 0);
    ASSERT_EQ(data.added().size(), 0);
}