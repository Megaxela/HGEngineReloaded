#pragma once

#include <Behaviour.hpp>

namespace HG::Physics::PlayRho::Behaviours
{
    /**
     * @brief Class, that displays debug info and controls
     * for physics controller.
     */
    class DebugControllerBehaviour : public HG::Core::Behaviour
    {
    public:

        /**
         * @brief Constructor.
         */
        DebugControllerBehaviour();

    protected:

        void onUpdate() override;

        void onStart() override;

    private:
        void tooltip(const char* text);
    };
}


