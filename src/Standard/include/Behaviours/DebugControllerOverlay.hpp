#pragma once

#include <Behaviour.hpp>

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Behaviour for controlling debug gameobjects
     * control.
     */
    class DebugControllerOverlay : public ::CORE_MODULE_NS::Behaviour
    {
    protected:

        void onUpdate() override;

    private:

        std::vector<::CORE_MODULE_NS::GameObject*> m_gameObjects;

    };
}


