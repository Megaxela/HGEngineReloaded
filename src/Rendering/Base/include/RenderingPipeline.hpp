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
        RenderingPipeline();

        /**
         * @brief Desturctor.
         */
        virtual ~RenderingPipeline();

        /**
         * @brief Method for setting system controller.
         * @tparam T Type of system controller. It must be
         * derived from `HG::Rendering::Base::SystemController`.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<T, SystemController>::value
        >::type setSystemController()
        {
            delete m_systemController;

            m_systemController = new T();
        };

        /**
         * @brief Method for getting system controller.
         * @return Pointer to system controller or
         * `nullptr` if there is no such.
         */
        SystemController* systemController() const;

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

    private:

        SystemController* m_systemController;
    };
}

