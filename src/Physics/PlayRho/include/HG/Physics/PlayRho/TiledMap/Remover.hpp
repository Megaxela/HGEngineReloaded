#pragma once

// HG::Core
#include <HG/Core/Logging.hpp>

// HG::Physics::Base
#include <HG/Physics/Base/PhysicsController.hpp>

// PlayRho
#include <PlayRho/PlayRho.hpp>

namespace HG::Physics::PlayRho::TiledMap
{
static void Remover(void* value, HG::Physics::Base::PhysicsController* physicsController)
{
    if (value == nullptr || physicsController == nullptr)
    {
        return;
    }

    auto body       = static_cast<playrho::d2::Body*>(value);
    auto controller = dynamic_cast<HG::Physics::PlayRho::Controller*>(physicsController);

    if (controller == nullptr)
    {
        HGErrorF() << "Can't remove body after physics controller change.";
        return;
    }

    controller->world()->Destroy(body);
}
} // namespace HG::Physics::PlayRho::TiledMap
