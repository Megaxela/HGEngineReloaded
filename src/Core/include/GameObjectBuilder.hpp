#pragma once

#include "GameObject.hpp"

namespace CORE_MODULE_NS
{
    /**
     * @brief Singleton builder
     * for game objects.
     */
    class GameObjectBuilder
    {
    public:

        // Disable copying
        GameObjectBuilder(const GameObjectBuilder&) = delete;
        GameObjectBuilder& operator=(const GameObjectBuilder&) = delete;

        /**
         * @brief Method for getting builder
         * instance.
         * @return Instance reference.
         */
        static GameObjectBuilder& i();

        /**
         * @brief Non explicit cast from builder
         * to gameobject. (Deploying)
         * @return Pointer to created game object.
         */
        operator GameObject*();

        /**
         * @brief Method for adding behaviour to
         * game object.
         * @param behaviour Pointer to behaviour.
         * @return Reference to builder.
         */
        GameObjectBuilder& addBehaviour(Behaviour* behaviour);

    private:
        /**
         * @brief Private constructor. (Singleton)
         */
        GameObjectBuilder();

        /**
         * @brief Method for picking gameobject
         * as current.
         */
        void pickCurrentGameObject();

        GameObject* m_currentGameObject;
    };
}


