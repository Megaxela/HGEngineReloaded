#pragma once

#include <cstdint>

namespace UTILS_MODULE_NS::PhysicalResource
{
    /**
     * @brief Method for getting total system
     * RAM.
     * @return Total RAM in bytes.
     */
    uint64_t getTotalRAM();

    /**
     * @brief Method for getting total system
     * free RAM.
     * @return Total free RAM in bytes.
     */
    uint64_t getFreeRAM();

    /**
     * @brief Method for getting RAM used by
     * this process.
     * @return Used RAM in bytes.
     */
    uint64_t getProcessRAMUsed();
};


