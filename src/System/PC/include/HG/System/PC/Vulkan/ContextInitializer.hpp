#pragma once

// HG::System::PC
#include <HG/System/PC/Base/ContextInitializer.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderingPipeline.hpp>

namespace HG::System::PC::Vulkan
{
    class ContextInitializer : public HG::System::PC::Base::ContextInitializer
    {
    public:
        static constexpr HG::Rendering::Base::RenderingPipeline::Backend SupportedBackend =
            HG::Rendering::Base::RenderingPipeline::Backend::Vulkan;

        void prepareWindowCreation() override;

        void windowSetup(GLFWwindow* window) override;

        Rendering::Base::SystemController::SystemControllerPipelineSetupCallback
        buildSystemControllerPipelineSetupFunction(GLFWwindow* window) override;

        Rendering::Base::SystemController::PipelineSetupCallback buildPipelineSetupFunction() override;
    };
}