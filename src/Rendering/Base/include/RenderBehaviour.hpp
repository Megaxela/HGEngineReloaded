#pragma once

#include <Behaviour.hpp>
#include <cstdio>
#include "RenderData.hpp"

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes cross pipeline
     * behaviour for rendering.
     */
    class RenderBehaviour : public HG::Core::Behaviour,
                            public RenderData
    {
    public:

        static constexpr std::size_t DataId = 1;

        /**
         * @brief Constructor.
         * @param type Derived class `typeid(*this).hash_code()` value.
         */
        explicit RenderBehaviour(std::size_t type);

        /**
         * @brief Method for getting behaviour type.
         * @return Real class `typeid(T).hash_code()` value.
         */
        std::size_t renderBehaviourType();

    protected:
        // Restrict to override this HG::Core::Behaviour methods

        void onStart() final;

        void onUpdate() final;

        void onFixedUpdate() final;

    private:

        std::size_t m_type;
    };
}

