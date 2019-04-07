#pragma once

// HG::Rendering::Base
#include <HG/Rendering/OpenGL/Forward/AbstractRenderer.hpp> // Required for inheritance

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/MaterialProcessor.hpp>

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
    class SpriteRenderer : public AbstractRenderer,
                           private HG::Rendering::OpenGL::Common::MaterialProcessor
    {
    public:

        /**
         * @brief Constructor.
         */
        SpriteRenderer();

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

    protected:

        /**
         * @brief Method for initializing sprite renderer.
         * This method creates buffers and materials.
         */
        void onInit() override;

        /**
         * @brief Method for deinitializing sprite renderer.
         * Clears buffers and removes materials.
         */
        void onDeinit() override;

    private:

        // Shader for sprite rendering
        HG::Rendering::Base::Material* m_spriteMaterial;

        // Sprite mesh
        Common::MeshData* m_spriteData;
    };
}


