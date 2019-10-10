#include <gtest/gtest.h>

// ALogger
#include <CurrentLogger.hpp>

class DumbLogger : public AbstractLogger
{
protected:
    void onNewMessage(const Message&) override
    {
    }
};

int main(int argc, char** argv)
{
    // Disable current logger warning
    CurrentLogger::setCurrentLogger(std::make_shared<DumbLogger>());

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
