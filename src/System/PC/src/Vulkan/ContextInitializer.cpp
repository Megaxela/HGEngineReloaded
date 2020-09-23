// HG::System::PC
#include <HG/System/PC/Vulkan/ContextInitializer.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

// GLFW
#include <GLFW/glfw3.h>

namespace HG::System::PC::Vulkan
{
    void ContextInitializer::prepareWindowCreation()
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    void ContextInitializer::windowSetup(GLFWwindow* window)
    {
        // noting to do
    }

    Rendering::Base::SystemController::SystemControllerPipelineSetupCallback
    ContextInitializer::buildSystemControllerPipelineSetupFunction(GLFWwindow* window)
    {
        return [window](void* instancePointer, void* result) {
            if (instancePointer == nullptr) {
                static auto message = "Vulkan: Backend specific initializer requires Vulkan instance to initialize surface";
                HGError(message);
                throw std::runtime_error(message);
            }

            vk::UniqueInstance& instance = *static_cast<vk::UniqueInstance*>(instancePointer);
            vk::SurfaceKHR& surfaceKhr = *static_cast<vk::SurfaceKHR*>(result);

            VkSurfaceKHR surfaceRaw;
            auto status = glfwCreateWindowSurface(*instance, window, nullptr, &surfaceRaw);

            if (status != VK_SUCCESS) {
                static auto message = fmt::format("Vulkan: Unable to setup window surface: {}", status);
                HGError(message);
                throw std::runtime_error(message);
            }

            surfaceKhr = vk::SurfaceKHR(surfaceRaw);
        };
    }

    Rendering::Base::SystemController::PipelineSetupCallback ContextInitializer::buildPipelineSetupFunction()
    {
        return [](void* data) {
            std::vector<const char*>& extensions = *static_cast<std::vector<const char*>*>(data);

            std::uint32_t amount = 0;
            const char** required_extensions = glfwGetRequiredInstanceExtensions(&amount);

            for (std::uint32_t i = 0; i < amount; ++i)
            {
                extensions.push_back(required_extensions[i]);
            }
        };
    }
}

