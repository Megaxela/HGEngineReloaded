// GTest
#include <gtest/gtest.h>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// spdlog
#include <spdlog/sinks/null_sink.h>

int main(int argc, char** argv)
{
    // Disable current logger warning
    spdlog::set_default_logger(spdlog::null_logger_st("null-logger"));

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
