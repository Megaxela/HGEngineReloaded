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

        GLFWwindow* m_window;

    };
}
#endif