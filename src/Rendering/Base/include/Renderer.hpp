#pragma once

#include <Scene.hpp>
#include "RenderingPipeline.hpp"
#include "Gizmos.hpp"
#include "MaterialCollection.hpp"

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes
     * layer between rendering pipeline and
     * user behaviour rendering system.
     */
    class Renderer
    {
    public:

        /**
         * @brief Default constructor.
         */
        explicit Renderer(::HG::Core::Application* application);

        /**
         * @brief Destructor.
         */
        ~Renderer();

        /**
         * @brief Method for setting pipeline
         * object.
         * @tparam PipelineType Type of pipeline object.
         */
        template<typename PipelineType>
        typename std::enable_if<
            std::is_base_of<RenderingPipeline, PipelineType>::value,
            PipelineType*
        >::type
        setPipeline()
        {
            m_pipeline = new PipelineType(m_parentApplication);
            return static_cast<PipelineType*>(m_pipeline);
        }

        /**
         * @brief Method for getting pipeline object.
         * @return Pointer to pipeline object.
         */
        RenderingPipeline* pipeline();

        /**
         * @brief Method for initializing pipeline.
         */
        bool init();

        /**
         * @brief Perform rendering finally.
         */
        void render(const ::HG::Core::Scene::GameObjectsContainer& gameObjects);

        /**
         * @brief Method for getting gizmos object.
         * @return Pointer to gizmos.
         */
        ::HG::Rendering::Base::Gizmos* gizmos();

        /**
         * @brief Method for getting renderer material collection.
         * @return Pointer to material collection.
         */
        ::HG::Rendering::Base::MaterialCollection* materialCollection();

        /**
         * @brief Method for setting up objects
         * with specified rendering pipeline.
         * @param obj Object.
         */
        template<typename T>
        void setup(T obj)
        {
            if (m_pipeline == nullptr)
            {
                Error() << "Can't setup rendering behaviour without set pipeline.";
                return;
            }

            m_pipeline->setup(obj);
        }

    private:

        ::HG::Core::Application* m_parentApplication;

        ::HG::Rendering::Base::RenderingPipeline* m_pipeline;

        ::HG::Rendering::Base::Gizmos m_gizmos;

        ::HG::Rendering::Base::MaterialCollection m_materialCollection;

    };
}

