#pragma once

#include "SystemController.hpp"
#include "Scene.hpp"

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes
     * abstract rendering pipeline.
     */
    class RenderingPipeline
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit RenderingPipeline(::CORE_MODULE_NS::Application* application);

        /**
         * @brief Desturctor.
         */
        virtual ~RenderingPipeline() = default;

        /**
         * @brief Method for initializing pipeline.
         * By default it just initialize system controller
         * and creates window.
         */
        virtual bool init();

        /**
         * @brief Rendering method.
         * @param objects Objects.
         */
        virtual void render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects) = 0;

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        ::CORE_MODULE_NS::Application* application() const;

    private:

        ::CORE_MODULE_NS::Application* m_parentApplication;
    };
}

