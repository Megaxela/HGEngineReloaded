#pragma once

#include <Rect.hpp>
#include <PhysicsController.hpp>
#include <Controller.hpp>
#include <CurrentLogger.hpp>

namespace PLAYRHO_PHYSICS_MODULE_NS::TiledMap
{
    static void* Creator(float x, float y, float w, float h,
                         PHYSICS_BASE_MODULE_NS::PhysicsController* physicsController)
    {
        auto castedController = dynamic_cast<PLAYRHO_PHYSICS_MODULE_NS::Controller*>(physicsController);

        if (castedController == nullptr)
        {
            ErrorF() << "Trying to create PlayRho collider, when another physics controller is used.";
            return nullptr;
        }

        auto body = castedController->world()->CreateBody(
            playrho::d2::BodyConf{}
                .UseLocation(playrho::Length2(x + w / 2, y - h / 2))
                .UseType(playrho::BodyType::Static)
        );

        body->CreateFixture(
            playrho::d2::Shape{
                playrho::d2::PolygonShapeConf{}.SetAsBox(w / 2, h / 2)
            }
        );

        return body;
    }
}


