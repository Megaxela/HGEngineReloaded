#pragma once

#include <Common/MeshData.hpp>
#include "AbstractRenderer.hpp"

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

        void init() override;

        void render(HG::Core::GameObject *gameObject,
                    HG::Rendering::Base::RenderBehaviour *renderBehaviour) override;

        size_t getTarget() override;

        void deinit() override;


    private:

        // Shader for sprite rendering
        HG::Rendering::Base::Material* m_spriteMaterial;

        // Sprite mesh
        Common::MeshData* m_spriteData;
    };
}


