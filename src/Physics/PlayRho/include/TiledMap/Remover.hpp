#pragma once

#include <PhysicsController.hpp>
#include <PlayRho/PlayRho.hpp>
#include <CurrentLogger.hpp>

namespace HG::Physics::PlayRho::TiledMap
{
    static void Remover(void* value, HG::Physics::Base::PhysicsController* physicsController)
    {
        if (value == nullptr || physicsController == nullptr)
        {
            return;
        }

        auto body = static_cast<playrho::d2::Body*>(value);
        auto controller = dynamic_cast<HG::Physics::PlayRho::Controller*>(physicsController);

        if (controller == nullptr)
        {
            ErrorF() << "Can't remove body after physics controller change.";
            return;
        }

        controller->world()->Destroy(body);
    }
}


