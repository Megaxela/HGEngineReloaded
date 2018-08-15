#pragma once

#include <Forward/AbstractRenderer.hpp>
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
    class MeshRenderer : public AbstractRenderer
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

        // Fallback mesh program
        HG::Rendering::Base::Material* m_meshFallbackMaterial;
    };
}


