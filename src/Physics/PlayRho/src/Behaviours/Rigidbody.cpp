#include <Controller.hpp>
#include <utility>
#include <imgui.h>
#include "Behaviours/Rigidbody.hpp"

PLAYRHO_PHYSICS_MODULE_NS::Behaviours::Rigidbody::Rigidbody() :
    m_configuration(),
    m_body(nullptr)
{

}

PLAYRHO_PHYSICS_MODULE_NS::Behaviours::Rigidbody::Rigidbody(playrho::d2::BodyConf bodyConfiguration) :
    m_configuration(std::move(bodyConfiguration)),
    m_body(nullptr)
{

}

PLAYRHO_PHYSICS_MODULE_NS::Behaviours::Rigidbody::Rigidbody(playrho::d2::Body *body) :
    m_configuration(),
    m_body(body)
{

}

void PLAYRHO_PHYSICS_MODULE_NS::Behaviours::Rigidbody::applyCurrentGameObjectTransform()
{
    if (!gameObject() || !m_body)
    {
        return;
    }

    playrho::Vec2 vector;

    auto position = gameObject()->transform()->globalPosition();

    vector[0] = position.x;
    vector[1] = position.y;

    m_body->SetTransform(vector, 0);
}

void PLAYRHO_PHYSICS_MODULE_NS::Behaviours::Rigidbody::onStart()
{
    if (!m_body)
    {
        m_body = scene()
            ->application()
            ->physicsController<Controller>()
            ->world()
            ->CreateBody(m_configuration);

        m_body->CreateFixture(
            playrho::d2::Shape{
                playrho::d2::PolygonShapeConf{}.SetAsBox(0.03f * 16 / 2, 0.03f * 16 / 2)
            }
        );
    }

    applyCurrentGameObjectTransform();
}

void PLAYRHO_PHYSICS_MODULE_NS::Behaviours::Rigidbody::onUpdate()
{
    if (!m_body)
    {
        Error() << "Can't update rigidbody without actual body.";
        return;
    }

    gameObject()->transform()->setGlobalPosition(
        glm::vec3(
            playrho::GetX(m_body->GetLocation()),
            playrho::GetY(m_body->GetLocation()),
            gameObject()->transform()->globalPosition().z
        )
    );

    gameObject()->transform()->setLocalRotation(
        glm::quat(glm::vec3(
            0.0f,
            0.0f,
            m_body->GetAngle()
        ))
    );
}

playrho::d2::Body *PLAYRHO_PHYSICS_MODULE_NS::Behaviours::Rigidbody::body() const
{
    return m_body;
}
