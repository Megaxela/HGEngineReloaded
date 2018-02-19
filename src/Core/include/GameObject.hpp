#pragma once

#include <DoubleBufferContainer.hpp>
#include "Transform.hpp"

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
         * @brief Method for calling gameobject
         * rendering behaviours.
         */
        void render();

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
        Transform* transform() const;

        /**
         * @brief Method for clearing gameobject.
         */
        void clear();

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

        UTILS_MODULE_NS::DoubleBufferContainer<Behaviour*> m_behaviours;

        Transform m_transform;

        std::string m_name;

        Scene* m_parentScene;
    };
}

