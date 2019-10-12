#include <gtest/gtest.h>

// HG::Utils
#include <HG/Utils/Logging.hpp>

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
    HG::Utils::Logging::overrideUserLogger(std::make_shared<DumbLogger>());

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
