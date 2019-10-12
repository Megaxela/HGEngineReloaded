// C++ STL
#include <utility>

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Scene.hpp>

// HG::Physics::PlayRho
#include <HG/Physics/PlayRho/Behaviours/Rigidbody.hpp>
#include <HG/Physics/PlayRho/Controller.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// ImGUI
#include <imgui.h>

namespace HG::Physics::PlayRho
{
Behaviours::Rigidbody::Rigidbody() : m_configuration(), m_body(nullptr)
{
}

Behaviours::Rigidbody::Rigidbody(playrho::d2::BodyConf bodyConfiguration) :
    m_configuration(std::move(bodyConfiguration)),
    m_body(nullptr)
{
}

Behaviours::Rigidbody::Rigidbody(playrho::d2::Body* body) : m_configuration(), m_body(body)
{
}

void Behaviours::Rigidbody::applyCurrentGameObjectTransform()
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

void Behaviours::Rigidbody::onStart()
{
    if (!m_body)
    {
        m_body = scene()->application()->physicsController<Controller>()->world()->CreateBody(m_configuration);

        m_body->CreateFixture(playrho::d2::Shape{
            playrho::d2::PolygonShapeConf{}.SetAsBox(0.03f * 14 / 2, 0.03f * 5 / 2, playrho::Length2(0, -0.2), 0)});
    }

    applyCurrentGameObjectTransform();
}

void Behaviours::Rigidbody::onUpdate()
{
    if (!m_body)
    {
        HGError() << "Can't update rigidbody without actual body.";
        return;
    }

    gameObject()->transform()->setGlobalPosition(glm::vec3(playrho::GetX(m_body->GetLocation()),
                                                           playrho::GetY(m_body->GetLocation()),
                                                           gameObject()->transform()->globalPosition().z));

    gameObject()->transform()->setLocalRotation(glm::quat(glm::vec3(0.0f, 0.0f, m_body->GetAngle())));
}

playrho::d2::Body* Behaviours::Rigidbody::body() const
{
    return m_body;
}
} // namespace HG::Physics::PlayRho
