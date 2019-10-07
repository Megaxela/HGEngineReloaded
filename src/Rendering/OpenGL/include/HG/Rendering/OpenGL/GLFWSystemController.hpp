#pragma once

// Only if using GLFW library

#ifndef GRAPHICS_USE_GLFW
#    warning "Trying to include GLFW system controller w/o GLFW renderer."
#endif

#ifdef GRAPHICS_USE_GLFW

#    include <HG/Rendering/Base/SystemController.hpp>
#    include <HG/Rendering/OpenGL/GL/gl_ordered.hpp>
#    include <imgui.h>

namespace HG::Rendering::OpenGL
{
/**
 * @brief Class, that describes system controller, using GLFW library.
 */
class GLFWSystemController : public HG::Rendering::Base::SystemController
{
public:
    /**
     * @brief Constructor.
     */
    explicit GLFWSystemController(HG::Core::Application* application);

    /**
     * @brief Method, that's closing window on destruction.
     */
    ~GLFWSystemController() override;

    /**
     * @brief Mehtod for initializing GLFW, setting GLFW settings and setting callbacks.
     * @return Success.
     */
    bool init() override;

    /**
     * @brief Method for creating window with defines width, height and title.
     * @param width Initial width.
     * @param height Initial height.
     * @param title Window title.
     * @return Success.
     */
    bool createWindow(uint32_t width, uint32_t height, std::string title) override;

    void changeTitle(std::string title) override;

    /**
     * @brief Method for swapping buffers in window.
     */
    void swapBuffers() override;

    /**
     * @brief Method, that's calling `glfwPollEvents`.
     */
    void pollEvents() override;

    Utils::Rect viewport() const override;

    void deinit() override;

    void closeWindow() override;

private:
    static void
    glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* mess, const void*);

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
     * @brief Method for handling gamepad events.
     */
    void handleGamepadsEvents();

    /**
     * @brief Method for handling window events.
     */
    void handleWindowEvents();

    /**
     * @brief GLFW callback for input.
     * @param c Scancode.
     */
    static void charCallback(GLFWwindow*, unsigned int c);

    /**
     * @brief GLFW callback for pressing keys.
     * @param key Key code.
     * @param action Action (Press/release)
     */
    static void keyPressCallback(GLFWwindow*, int key, int, int action, int);

    /**
     * @brief GLFW callback for moving cursor.
     * @param x X coordinate, locally to window.
     * @param y Y coordinate, locally to window.
     */
    static void cursorPosCallback(GLFWwindow*, double x, double y);

    /**
     * @brief GLFW callback for connecting/disconnecting
     * gamepads.
     * @param gamepad Gamepad index.
     * @param event Event.
     */
    static void joystickCallback(int gamepad, int event);

    /**
     * @brief GLFW callback for mouse buttons.
     * @param button Mouse button index.
     * @param action Action code.
     */
    static void mouseButtonCallback(GLFWwindow*, int button, int action, int);

    /**
     * @brief GLFW callback for mouse wheel.
     * @param xDelta X delta.
     * @param yDelta Y delta.
     */
    static void mouseWheelCallback(GLFWwindow*, double xDelta, double yDelta);

    /**
     * @brief GLFW callback for framebuffer size changed.
     * @param width New width.
     * @param height New height.
     */
    static void framebufferSizeCallback(GLFWwindow*, int width, int height);

    GLFWwindow* m_window;

    GLFWcursor* m_mouseCursors[ImGuiMouseCursor_COUNT];
};
} // namespace HG::Rendering::OpenGL
#endif