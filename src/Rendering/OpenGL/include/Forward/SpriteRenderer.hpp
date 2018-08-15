#pragma once

#include <Forward/AbstractRenderer.hpp>

namespace HG::Rendering::OpenGL::Common
{
    class MeshData;
}

namespace HG::Rendering::OpenGL::Forward
{
    /**
     * @brief Class, that describes forward
     * sprite renderer.
     */
    class SpriteRenderer : public AbstractRenderer
    {
    public:

        /**
         * @brief Constructor.
         */
        SpriteRenderer();

        /**
         * @brief Destructor.
         */
        ~SpriteRenderer() override;

        /**
         * @brief Method for initializing sprite renderer.
         * This method creates buffers and materials.
         */
        void init() override;

        /**
         * @brief Method for rendering sprite rendering behaviour.
         * @param gameObject Rendering behaviour owner.
         * @param renderBehaviour Actual rendering behaviour.
         */
        void render(HG::Rendering::Base::RenderBehaviour *renderBehaviour) override;

        /**
         * @brief What render behaviours can proceed this
         * renderer. (HG::Rendering::Base::Behaviours::Sprite)
         */
        size_t getTarget() override;

        /**
         * @brief Method for deinitializing sprite renderer.
         * Clears buffers and removes materials.
         */
        void deinit() override;

    private:

        // Shader for sprite rendering
        HG::Rendering::Base::Material* m_spriteMaterial;

        // Sprite mesh
        Common::MeshData* m_spriteData;
    };
}


