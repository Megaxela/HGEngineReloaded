#pragma once

// C++ STL
#include <cstdlib>

// HG::Rendering::Base
#include <HG/Rendering/Base/MaterialValue.hpp>

// HG::Utils
#include <HG/Utils/Interfaces/Initializable.hpp>

// gl
#include <gl/all.hpp>

namespace HG::Core
{
    class Application;
}

namespace HG::Rendering::Base
{
    class RenderBehaviour;
    class Material;
}

namespace HG::Rendering::OpenGL::Forward
{
    /**
     * @brief Class, that describes abstract
     * rendering behaviours renderer for forward
     * rendering pipeline.
     */
    class AbstractRenderer : public HG::Utils::Interfaces::Initializable
    {
    public:

        /**
         * @brief Constructor.
         */
        AbstractRenderer();

        /**
         * @brief Method for rendering some behaviour.
         * @param gameObject Behaviour owner.
         * @param renderBehaviour Rendering behaviour.
         */
        virtual void render(HG::Rendering::Base::RenderBehaviour* renderBehaviour) = 0;

        /**
         * @brief Method, that will be used by forward renderer
         * to identify what behaviour type will be proceed by
         * implementation.
         * @return Behaviour type.
         */
        virtual std::size_t getTarget() = 0;

        /**
         * @brief Method for getting application.
         * @return Pointer to application.
         */
        HG::Core::Application* application() const;

        /**
         * @brief Method for setting application.
         * @param application Pointer to application.
         */
        void setApplication(HG::Core::Application* application);

    private:
        HG::Core::Application* m_application;
    };
}


