#pragma once

#include <cstdio>

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

    private:

        std::size_t m_type;
    };
}

