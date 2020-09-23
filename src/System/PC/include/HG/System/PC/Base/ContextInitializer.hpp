#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/SystemController.hpp>

// forward declaration
struct GLFWwindow;

namespace HG::System::PC::Base
{
    /**
     * @brief Class, that performs rendering backend specific
     * context initialization.
     */
    class ContextInitializer
    {
    public:
        /**
         * @brief This method is called before window creation.
         * It can be used to set rendering backend specific hints for
         * GLFW.
         */
        virtual void prepareWindowCreation() = 0;

        /**
         * @brief This method is called after window creation and
         * basic setup. It can be used to create contexts.
         */
        virtual void windowSetup(GLFWwindow* window) = 0;

        /**
         * @brief This method should provides function for
         * rendering pipeline to execute. It can return nullptr
         * if no actions required from rendering pipeline side.
         */
        virtual HG::Rendering::Base::SystemController::SystemControllerPipelineSetupCallback buildSystemControllerPipelineSetupFunction(GLFWwindow* window) = 0;

        /**
         * @brief This method should provides function for
         * rendering pipeline to execute and receive some
         * useful data for initialization.
         * It can return nullptr if no actions required from rendering pipeline side.
         */
        virtual HG::Rendering::Base::SystemController::PipelineSetupCallback buildPipelineSetupFunction() = 0;
    };
}