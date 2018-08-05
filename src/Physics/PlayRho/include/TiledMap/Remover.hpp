#pragma once

#include <PhysicsController.hpp>
#include <PlayRho/PlayRho.hpp>
#include <CurrentLogger.hpp>

namespace PLAYRHO_PHYSICS_MODULE_NS::TiledMap
{
    static void Remover(void* value, PHYSICS_BASE_MODULE_NS::PhysicsController* physicsController)
    {
        if (value == nullptr || physicsController == nullptr)
        {
            return;
        }

        auto body = static_cast<playrho::d2::Body*>(value);
        auto controller = dynamic_cast<PLAYRHO_PHYSICS_MODULE_NS::Controller*>(physicsController);

        if (controller == nullptr)
        {
            ErrorF() << "Can't remove body after physics controller change.";
            return;
        }

        controller->world()->Destroy(body);
    }
}


