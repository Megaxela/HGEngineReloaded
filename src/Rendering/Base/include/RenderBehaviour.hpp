#pragma once

#include <cstdio>
#include <GameObject.hpp>

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes cross pipeline
     * behaviour for rendering.
     */
    class RenderBehaviour
    {
    public:

        /**
         * @brief Constructor.
         * @param type Derived class `typeid(*this).hash_code()` value.
         */
        explicit RenderBehaviour(std::size_t type);

        /**
         * @brief Default virtual destructor.
         */
        virtual ~RenderBehaviour() = default;

        /**
         * @brief Method for getting behaviour type.
         * @return Real class `typeid(T).hash_code()` value.
         */
        std::size_t behaviourType();

        /**
         * @brief Method for getting parent gameobject.
         * @return Parent gameobject or nullptr if
         * there is no parent.
         */
        ::CORE_MODULE_NS::GameObject* gameObject();

    protected:
        friend class ::CORE_MODULE_NS::GameObject;

        /**
         * @brief Method for gameobject to set parent gameobject.
         * @param gameObject
         */
        void setParentGameObject(::CORE_MODULE_NS::GameObject* gameObject);

    private:

        std::size_t m_type;

        ::CORE_MODULE_NS::GameObject* m_parent;
    };
}

