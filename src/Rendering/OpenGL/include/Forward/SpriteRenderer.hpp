#pragma once

#include <Common/MeshData.hpp>
#include "AbstractRenderer.hpp"

namespace OGL_RENDERING_MODULE_NS::Forward
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

        void init() override;

        void render(CORE_MODULE_NS::GameObject *gameObject,
                    RENDERING_BASE_MODULE_NS::RenderBehaviour *renderBehaviour) override;

        size_t getTarget() override;


    private:

        // Shader for sprite rendering
        ::RENDERING_BASE_MODULE_NS::Material* m_spriteMaterial;

        // Sprite mesh
        Common::MeshData* m_spriteData;
    };
}


