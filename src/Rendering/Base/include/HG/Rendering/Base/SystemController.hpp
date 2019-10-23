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
    virtual ~SystemController();

    /**
     * @brief Method for initializing graphics
     * subsystem.
     */
    bool init();

    /**
     * @brief Method for deinitializing graphics
     * subsystem.
     */
    void deinit();

    /**
     * @brief Method for creating window.
     * @param width Window width.
     * @param height Window height.
     * @param title Window title.
     */
    bool createWindow(uint32_t width, uint32_t height, std::string title);

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
     * @brief Method for checking is window focused.
     * @return Is window focused.
     */
    virtual bool isWindowFocused() = 0;

    /**
     * @brief Method for swapping buffers in window.
     */
    virtual void swapBuffers() = 0;

    /**
     * @brief Method for polling events.
     */
    void pollEvents();

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
    [[nodiscard]] HG::Core::Application* application() const;

protected:
    /**
     * @brief Derived defined events polling, that has to populate
     * HG::Core::Input
     */
    virtual void onPollEvents() = 0;

    /**
     * @brief Derived defined window/context creation function.
     */
    virtual bool onCreateWindow(std::uint32_t width, std::uint32_t height, std::string title) = 0;

    /**
     * @brief Derived defined init method, that will be called at the application beginning.
     * @return Initialization success.
     */
    virtual bool onInit() = 0;

    /**
     * @brief Derived defined deinit method, that will be called before application close or
     * renderer change.
     */
    virtual void onDeinit() = 0;

private:
    /**
     * @brief Method for initializing ImGui system.
     */
    void imGuiInit();

    /**
     * @brief Method for deinitializing ImGui system.
     */
    void imGuiDeinit();

    /**
     * @brief Method for preparing ImGui to new frame.
     */
    void imGuiNewFrame();

    /**
     * @brief Method for reading keys from HG::Core::Input into ImGui backend.
     */
    void imGuiReadKeys();

    HG::Core::Application* m_application;
};
} // namespace HG::Rendering::Base
