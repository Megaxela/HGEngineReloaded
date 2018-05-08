#pragma once

// Only if using GLFW library
#include <SystemController.hpp>
#include <GL/gl_ordered.hpp>

#ifdef GRAPHICS_USE_GLFW

namespace OGL_RENDERING_MODULE_NS
{
    /**
     * @brief Class, that describes system controller, using GLFW library.
     */
    class GLFWSystemController : public RENDERING_BASE_MODULE_NS::SystemController
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit GLFWSystemController(::CORE_MODULE_NS::Application* application);

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

        /**
         * @brief Method for swapping buffers in window.
         */
        void swapBuffers() override;

        /**
         * @brief Method, that's calling `glfwPollEvents`.
         */
        void pollEvents() override;

    private:

        /**
         * @brief Method for handling gamepad events.
         */
        void handleGamepadsEvents();

        /**
         * @brief Method for handling window events.
         */
        void handleWindowEvents();

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
        static void cursorPosCallback(GLFWwindow *, double x, double y);

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
         * @brief GLFW callback for framebuffer size changed.
         * @param width New width.
         * @param height New height.
         */
        static void framebufferSizeCallback(GLFWwindow*, int width, int height);

        GLFWwindow* m_window;

    };
}
#endif