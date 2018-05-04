#pragma once

// Only if using GLFW library
#include <SystemController.hpp>
#include <GL/gl_ordered.hpp>

#ifdef GRAPHICS_USE_GLFW

namespace RENDERING_OGL_MODULE_NS
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
        GLFWSystemController();

        /**
         * @brief Method, that's closing window on destruction.
         */
        ~GLFWSystemController() override;

        bool init() override;

        bool createWindow(uint32_t width, uint32_t height, std::string title) override;

    private:

        GLFWwindow* m_window;

    };
}
#endif