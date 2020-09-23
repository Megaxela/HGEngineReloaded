// HG::System::PC
#include <HG/System/PC/OpenGL/ContextInitializer.hpp>

// GLFW
#include <GLFW/glfw3.h>

namespace HG::System::PC::OpenGL
{
    void ContextInitializer::prepareWindowCreation()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }

    void ContextInitializer::windowSetup(GLFWwindow* window)
    {
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
    }

    HG::Rendering::Base::SystemController::SystemControllerPipelineSetupCallback
    ContextInitializer::buildSystemControllerPipelineSetupFunction(GLFWwindow*)
    {
        return nullptr;
    }

    Rendering::Base::SystemController::PipelineSetupCallback ContextInitializer::buildPipelineSetupFunction()
    {
        return nullptr;
    }
}