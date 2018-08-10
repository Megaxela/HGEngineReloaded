#pragma once

#include <RenderBehaviour.hpp>
#include <cstdlib>
#include <Application.hpp>
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Forward
{
    /**
     * @brief Class, that describes abstract
     * rendering behaviours renderer for forward
     * rendering pipeline.
     */
    class AbstractRenderer
    {
    public:

        /**
         * @brief Constructor.
         */
        AbstractRenderer();

        /**
         * @brief Destructor.
         */
        virtual ~AbstractRenderer() = default;

        /**
         * @brief Method for initializing renderer.
         */
        virtual void init() = 0;

        /**
         * @brief Method for rendering some behaviour.
         * @param gameObject Behaviour owner.
         * @param renderBehaviour Rendering behaviour.
         */
        virtual void render(HG::Core::GameObject* gameObject,
                            HG::Rendering::Base::RenderBehaviour* renderBehaviour) = 0;

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

    protected:

        /**
         * @brief Method for applying shader uniforms in material.
         * @param material Material.
         */
        void applyShaderUniforms(HG::Rendering::Base::Material* material);

        void setShaderUniform(gl::program *program,
                              const std::string &name,
                              const HG::Rendering::Base::Material::Value &value);

    private:

        HG::Core::Application* m_application;

        GLuint m_textureNumber;
    };
}


