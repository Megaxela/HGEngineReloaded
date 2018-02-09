#pragma once

#include <UniqueMergeContainer.hpp>

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

        UTILS_MODULE_NS::UniqueMergeContainer<Behaviour*> m_behaviours;

        Scene* m_parentScene;
    };
}

