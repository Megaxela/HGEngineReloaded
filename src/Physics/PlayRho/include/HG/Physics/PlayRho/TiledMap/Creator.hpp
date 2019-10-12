#pragma once

// HG::Core
#include <HG/Core/Logging.hpp>

// HG::Utils
#include <HG/Utils/Rect.hpp>

// HG::Physics::Base
#include <HG/Physics/Base/PhysicsController.hpp>

// HG::Physics::PlayRho
#include <Controller.hpp>

namespace HG::Physics::PlayRho::TiledMap
{
class Creator
{
public:
    void* createRectangle(float x, float y, float w, float h, HG::Physics::Base::PhysicsController* physicsController)
    {
        auto castedController = dynamic_cast<HG::Physics::PlayRho::Controller*>(physicsController);

        if (castedController == nullptr)
        {
            HGError() << "Trying to create PlayRho collider, when another physics controller is used.";
            return nullptr;
        }

        auto body = castedController->world()->CreateBody(playrho::d2::BodyConf{}
                                                              .UseLocation(playrho::Length2(x + w / 2, y - h / 2))
                                                              .UseType(playrho::BodyType::Static));

        body->CreateFixture(playrho::d2::Shape{playrho::d2::PolygonShapeConf{}.SetAsBox(w / 2, h / 2)});

        return body;
    }

    void* createEllipse(float x, float y, float w, float h, HG::Physics::Base::PhysicsController* physicsController)
    {
        auto castedController = dynamic_cast<HG::Physics::PlayRho::Controller*>(physicsController);

        if (castedController == nullptr)
        {
            HGError() << "Trying to create PlayRho collider, when another physics controller is used.";
            return nullptr;
        }

        auto body = castedController->world()->CreateBody(playrho::d2::BodyConf{}
                                                              .UseLocation(playrho::Length2(x + w / 2, y - h / 2))
                                                              .UseType(playrho::BodyType::Static));

        // If it's circle - use optimized
        // shape. Otherwise build ellipse
        if (glm::abs(w - h) < 0.001)
        {
            body->CreateFixture(playrho::d2::Shape{playrho::d2::DiskShapeConf{}.UseRadius(w / 2)});
        }
        else
        {
            playrho::d2::VertexSet set;

            auto parts = 24;
            glm::vec2 radiuses(w / 2, h / 2);

            for (uint32_t i = 0; i < parts; ++i)
            {
                auto angleRadians = (2 * glm::pi<float>()) / parts * i;

                /*
                 *                        ab
                 * r = -----------------------------------------
                 *      sqrt(a^2 * sin^2(th) + b^2 * cos^2(th))
                 */
                // Sick optimizations (no)
                auto angleSin = glm::sin(angleRadians);
                auto angleCos = glm::cos(angleRadians);

                auto radius = (radiuses.x * radiuses.y) / glm::sqrt(radiuses.x * radiuses.x * angleSin * angleSin +
                                                                    radiuses.y * radiuses.y * angleCos * angleCos);

                set.add(playrho::Length2{angleCos * radius, angleSin * radius});
            }

            body->CreateFixture(playrho::d2::Shape{playrho::d2::PolygonShapeConf{}.Set(set)});
        }

        return body;
    }

    void* createPolygon(float x,
                        float y,
                        const std::vector<glm::vec2>& points,
                        HG::Physics::Base::PhysicsController* physicsController)
    {
        auto castedController = dynamic_cast<HG::Physics::PlayRho::Controller*>(physicsController);

        if (castedController == nullptr)
        {
            HGError() << "Trying to create PlayRho collider, when another physics controller is used.";
            return nullptr;
        }

        auto body = castedController->world()->CreateBody(
            playrho::d2::BodyConf{}.UseLocation(playrho::Length2(x, y)).UseType(playrho::BodyType::Static));

        playrho::d2::VertexSet set;

        for (auto&& point : points)
        {
            set.add(playrho::Length2(point.x, point.y));
        }

        body->CreateFixture(playrho::d2::Shape{playrho::d2::PolygonShapeConf{}.Set(set)});

        return body;
    }
};
} // namespace HG::Physics::PlayRho::TiledMap
