#pragma once

#include <GameObject.hpp>

namespace CORE_MODULE_NS
{
    /**
     * @brief Class, that describes abstract
     * gameobject behaviour component.
     */
    class Behaviour
    {
    public:

        /**
         * @brief Constructor.
         */
        Behaviour();

        /**
         * @brief Destructor.
         */
        virtual ~Behaviour() = default;

        /**
         * @brief Public method for calling
         * overridden update methods.
         */
        void update();

        /**
         * @brief Public method for calling
         * start method.
         */
        void start();

        /**
         * @brief Method for getting gameobject.
         * @return Pointer to parent gameobject.
         */
        GameObject* gameObject() const;

        /**
         * @brief Method for receiving parent scene.
         * @return Scene.
         */
        Scene* scene() const;

    protected:

        /**
         * @brief Virtual method, that's called
         * once on scene starting.
         */
        virtual void onStart();

        /**
         * @brief Virtual method, that's called
         * every frame.
         */
        virtual void onUpdate();

        /**
         * @brief Virtual method, that's called
         * after fixed time. (If base framerate more
         * than fixed update interval)
         */
        virtual void onFixedUpdate();

        friend class GameObject;

        /**
         * @brief Method for setting parent gameobject.
         * @param ptr Parent gameobject.
         */
        void setParentGameObject(GameObject* ptr);

    private:

        GameObject* m_parent;
    };
}

