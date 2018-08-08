#pragma once

#include <Material.hpp>
#include "AbstractRenderer.hpp"
#include <gl/all.hpp>

namespace OGL_RENDERING_MODULE_NS::Forward
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

        void render(CORE_MODULE_NS::GameObject *gameObject,
                    RENDERING_BASE_MODULE_NS::RenderBehaviour *renderBehaviour) override;

        size_t getTarget() override;

        void init() override;

    private:

        // Fallback mesh program
        ::RENDERING_BASE_MODULE_NS::Material* m_meshFallbackMaterial;
    };
}


