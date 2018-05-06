#pragma once

#include <cstdint>
#include <string>

namespace CORE_MODULE_NS
{
    class Application;
}

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
         * @brief Constructor.
         * @param application Pointer to parent application.
         */
        explicit SystemController(::CORE_MODULE_NS::Application* application);

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

        /**
         * @brief Method for swapping buffers in window.
         */
        virtual void swapBuffers() = 0;

        /**
         * @brief Method for polling events.
         */
        virtual void pollEvents() = 0;

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        ::CORE_MODULE_NS::Application* application() const;

    private:

        ::CORE_MODULE_NS::Application* m_application;
    };
}

