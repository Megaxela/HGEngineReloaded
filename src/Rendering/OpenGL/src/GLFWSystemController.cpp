#include <CurrentLogger.hpp>
#include "GLFWSystemController.hpp"

#ifdef GRAPHICS_USE_GLFW

RENDERING_OGL_MODULE_NS::GLFWSystemController::GLFWSystemController() :
    m_window(nullptr)
{

}

RENDERING_OGL_MODULE_NS::GLFWSystemController::~GLFWSystemController()
{
    glfwTerminate();
    m_window = nullptr;
}

bool RENDERING_OGL_MODULE_NS::GLFWSystemController::init()
{
    Info() << "Initializing GLFW";

    // Setting error callback
    glfwSetErrorCallback(
        [](int code, const char* description)
        {
            ErrorF() << "GLFW Received error #" << code << ", " << description;
        }
    );

    // Initializing GLFW
    if (!glfwInit())
    {
        return false;
    }

    // Setting OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

bool RENDERING_OGL_MODULE_NS::GLFWSystemController::createWindow(uint32_t width, uint32_t height, std::string title)
{
    Info() << "Creating window " << width << "x" << height << " with title \"" << title << "\"";

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    return true;
}

#endif