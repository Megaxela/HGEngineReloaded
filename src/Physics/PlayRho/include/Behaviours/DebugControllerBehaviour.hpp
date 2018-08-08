#pragma once

#include <Behaviour.hpp>

namespace PLAYRHO_PHYSICS_MODULE_NS::Behaviours
{
    /**
     * @brief Class, that displays debug info and controls
     * for physics controller.
     */
    class DebugControllerBehaviour : public CORE_MODULE_NS::Behaviour
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


