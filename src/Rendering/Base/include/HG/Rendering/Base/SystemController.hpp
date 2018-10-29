#pragma once

// C++ STL
#include <cstdint>
#include <string>

// HG::Utils
#include <HG/Utils/Rect.hpp>

namespace HG::Core
{
    class Application;
}

namespace HG::Rendering::Base
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
        explicit SystemController(HG::Core::Application* application);

        // Disable copying
        SystemController(const SystemController&) = delete;
        SystemController& operator=(const SystemController&) = delete;

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
         * @brief Method for deinitializing graphics
         * subsystem.
         */
        virtual void deinit() = 0;

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
         * @brief Method for changing window title.
         * @param title New window title.
         */
        virtual void changeTitle(std::string title) = 0;

        /**
         * @brief Method for closing opened window.
         */
        virtual void closeWindow() = 0;

        /**
         * @brief Method for swapping buffers in window.
         */
        virtual void swapBuffers() = 0;

        /**
         * @brief Method for polling events.
         */
        virtual void pollEvents() = 0;

        /**
         * @brief Method for getting current viewport.
         * Camera uses this method to determine initial
         * viewport at `start` stage.
         * @return Viewport.
         */
        virtual HG::Utils::Rect viewport() const = 0;

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Core::Application* application() const;

    private:

        HG::Core::Application* m_application;
    };
}

