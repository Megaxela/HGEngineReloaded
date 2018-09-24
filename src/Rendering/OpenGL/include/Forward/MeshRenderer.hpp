#pragma once

// HG::Rendering::OpenGL
#include <Forward/AbstractRenderer.hpp> // Required for inheritance

// gl
#include <gl/all.hpp>

namespace HG::Rendering::Base
{
    class Material;
}

namespace HG::Rendering::OpenGL::Forward
{
    /**
     * @brief Class, that describes forward mesh
     * renderer.
     */
class MeshRenderer : public HG::Rendering::OpenGL::Forward::AbstractRenderer
    {
    public:

        /**
         * @brief Constructor.
         */
        MeshRenderer();

        /**
         * @brief Destructor.
         */
        virtual ~MeshRenderer();

        /**
         * @brief Method for performs actual mesh rendering.
         * @param renderBehaviour Render behaviour.
         */
        void render(HG::Rendering::Base::RenderBehaviour *renderBehaviour) override;

        /**
         * @brief Method for getting render behaviours type, that
         * this renderer can proceed.
         * (HG::Rendering::Base::Behaviours::Mesh)
         */
        size_t getTarget() override;

        /**
         * @brief Method for initializing mesh renderer.
         * Loads material.
         */
        void init() override;

        /**
         * @brief Method for deinitializing mesh renderer.
         * Destroys material.
         */
        void deinit() override;

    private:

        struct LightPointUniformNames
        {
            LightPointUniformNames(std::string position,
                                   std::string constant,
                                   std::string linear,
                                   std::string quadratic,
                                   std::string diffuse) :
                position(std::move(position)),
                constant(std::move(constant)),
                linear(std::move(linear)),
                quadratic(std::move(quadratic)),
                diffuse(std::move(diffuse))
            {

            }

            std::string position;
            std::string constant;
            std::string linear;
            std::string quadratic;
            std::string diffuse;
        };

        // Fallback mesh program
        HG::Rendering::Base::Material* m_meshFallbackMaterial;

        std::vector<LightPointUniformNames> m_pointLightNames;
    };
}


