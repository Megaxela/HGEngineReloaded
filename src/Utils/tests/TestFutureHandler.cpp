// C++ STL
#include <thread>

// HG::Utils
#include <HG/Utils/FutureHandler.hpp>

// GTest
#include <gtest/gtest.h>

struct Object
{
    Object() : data(0)
    {}

    Object(int data) : data(data)
    {}

    int getData()
    {
        return data;
    }

    int data;
};

template<typename T>
std::shared_future<Object*> getSharedFuture(T sleep_for, Object* result_value)
{
    auto promise = std::make_shared<std::promise<Object*>>();
    std::shared_future<Object*> future(promise->get_future());

    std::thread thread(
            [promise, sleep_for, result_value]()
            {
                std::this_thread::sleep_for(sleep_for);

                promise->set_value(result_value);
            }
    );

    thread.detach();

    return future;
}

TEST(Utils, FutureHandler)
{
    Object expected = 0xDEAD;
    HG::Utils::FutureHandler futureHandler(getSharedFuture(
            std::chrono::seconds(1),
            &expected
    ));

    HG::Utils::FutureHandler copy = futureHandler;

    ASSERT_EQ(copy.get(), nullptr);

    auto* result = copy.guaranteeGet();

    ASSERT_EQ(copy->getData(), expected.getData());
    ASSERT_EQ(result, &expected);
}

TEST(Utils, FutureHandlerPredefined)
{
    HG::Utils::FutureHandler predefined(12);

    ASSERT_EQ(predefined.get(), 12);
    ASSERT_EQ(predefined.guaranteeGet(), 12);
}