#pragma once

#include <cstdint>
#include <string>

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes abstract rendering
     * system controller. That's operates creating windows
     * and etc.
     */
    class SystemController
    {
    public:

        /**
         * @brief Destructor.
         */
        virtual ~SystemController() = default;

        /**
         * @brief Method for initializing graphics
         * subsystem.
         */
        virtual bool init() = 0;

        /**
         * @brief Method for creating window.
         * @param width Window width.
         * @param height Window height.
         * @param title Window title.
         */
        virtual bool createWindow(uint32_t width,
                                  uint32_t height,
                                  std::string title) = 0;
    };
}

