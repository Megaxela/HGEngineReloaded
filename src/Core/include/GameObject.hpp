#pragma once

#include <DoubleBufferContainer.hpp>
#include "Transform.hpp"

namespace RENDERING_BASE_MODULE_NS
{
    class RenderBehaviour;
}

namespace CORE_MODULE_NS
{
    // Forward declaration
    class Behaviour;
    class Scene;

    /**
     * @brief Class, that describes
     * abstract scene actor.
     */
    class GameObject
    {
    public:

        /**
         * @brief Method for updating gameobject
         * behaviours. Also this method is executing
         * `onStart` method if gameobject was recently
         * added.
         */
        void update();

        /**
         * @brief Method for getting parent scene.
         * @return Pointer to parent scene.
         */
        Scene* scene() const;

        /**
         * @brief Method for adding new behaviour.
         * @param behaviour New behaviour.
         */
        void addBehaviour(Behaviour* behaviour);

        /**
         * @brief Method for adding new render behaviour.
         * For example MeshRenderer.
         * @param renderBehaviour Render behaviour.
         */
        void addRenderingBehaviour(::RENDERING_BASE_MODULE_NS::RenderBehaviour* renderBehaviour);

        /**
         * @brief Method for setting internal game object
         * name.
         * @param name Name.
         */
        void setName(std::string name);

        /**
         * @brief Method for getting game object
         * name.
         * @return String name.
         */
        std::string name() const;

        /**
         * @brief Method for getting game object
         * transform.
         * @return Pointer to transform.
         */
        Transform* transform();

        /**
         * @brief Method for clearing gameobject.
         */
        void clear();

        /**
         * @brief Method for receiving
         * rendering behaviours of gameobject.
         * @tparam Container Container type.
         * @param container Container object.
         */
        template<typename Container>
        void findRenderingBehaviours(Container& container)
        {
            for (auto&& behaviour : m_renderBehaviours)
            {
                container.push_back(behaviour);
            }
        }


    protected:

        friend class Scene;

        /**
         * @brief Method for setting parent scene.
         * @param parent Parent scene.
         */
        void setParentScene(Scene* parent);

    private:

        friend class GameObjectCache;

        /**
         * @brief Destructor only for GameObjectBuilder.
         */
        ~GameObject();

        /**
         * @brief Constructor.
         */
        GameObject();

        ::UTILS_MODULE_NS::DoubleBufferContainer<Behaviour*> m_behaviours;
        ::UTILS_MODULE_NS::DoubleBufferContainer<::RENDERING_BASE_MODULE_NS::RenderBehaviour*> m_renderBehaviours;

        Transform m_transform;

        std::string m_name;

        Scene* m_parentScene;
    };
}

