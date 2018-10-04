// HG::Core
#include <TimeStatistics.hpp>
#include <Application.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>
#include <Input.hpp>
#include <Transform.hpp>

// HG::Rendering::Base
#include <Camera.hpp>

// HG::Standard
#include <Behaviours/FPSCameraMovement.hpp>

// ALogger
#include <CurrentLogger.hpp>

// GLM
#include <glm/glm.hpp>

HG::Standard::Behaviours::FPSCameraMovement::FPSCameraMovement() :
    m_enabled(false),
    m_first(false),
    m_lastMousePosition(),
    m_front(glm::vec3(0.0f, 0.0f, 1.0f)),
    m_yaw(90),
    m_pitch(0),
    m_camera(nullptr)
{

}

void HG::Standard::Behaviours::FPSCameraMovement::onStart()
{
    m_camera = gameObject()->findBehaviour<HG::Rendering::Base::Camera>();

    if (!m_camera)
    {
        Warning() << "Can't setup FPS camera movement, without camera.";
    }
}

void HG::Standard::Behaviours::FPSCameraMovement::onUpdate()
{
    if (m_camera == nullptr)
    {
        return;
    }

    if (input()->keyboard()->isPushed(
        HG::Core::Input::Keyboard::Key::R
    ))
    {
        m_enabled = !m_enabled;

        if (m_enabled)
        {
            m_lastMousePosition = input()->mouse()->getMousePosition();
        }

//        input()->mouse()->setCursorHidden  (m_enabled);
        input()->mouse()->setCursorDisabled(m_enabled);
    }

    if (input()->keyboard()->isPushed(
        HG::Core::Input::Keyboard::Key::ESC
    ))
    {
        m_enabled = false;

        input()->mouse()->setCursorHidden  (false);
        input()->mouse()->setCursorDisabled(false);
    }

    if (m_enabled)
    {
        handleMouseRotation();

        handleKeyboardMovement();
    }
}

void HG::Standard::Behaviours::FPSCameraMovement::handleMouseRotation()
{
    // Frame sens
    float frameSensitivity =
        m_propertySensitivity / 10;

    // Mouse
    auto mousePosition = input()->mouse()->getMousePosition();
    mousePosition -= m_lastMousePosition;

    m_yaw += mousePosition.x * frameSensitivity;
    m_pitch += mousePosition.y * frameSensitivity;

    m_lastMousePosition = input()->mouse()->getMousePosition();

    if (m_pitch >= 89.0f)
    {
        m_pitch = 89.0f;
    }

    if (m_pitch <= -89.0f)
    {
        m_pitch = -89.0f;
    }

    m_front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_front.y = std::sin(glm::radians(m_pitch));
    m_front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);

    m_camera->lookAt(
        m_camera->gameObject()->transform()->localPosition() + m_front
    );
}

void HG::Standard::Behaviours::FPSCameraMovement::handleKeyboardMovement()
{
    auto dt = scene()
                  ->application()
                  ->timeStatistics()
                  ->lastFrameDeltaTime().count() / 1000000.0;

    auto input = scene()->application()->input()->keyboard();

    glm::vec3 inputDirection(0.0f, 0.0f, 0.0f);

    auto speed = static_cast<float>(m_propertyMovementSpeed * dt);

    if (input->isPressed(HG::Core::Input::Keyboard::Key::Q))
    {
        inputDirection.y -= speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::E))
    {
        inputDirection.y += speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::W))
    {
        inputDirection.z -= speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::S))
    {
        inputDirection.z += speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::A))
    {
        inputDirection.x -= speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::D))
    {
        inputDirection.x += speed;
    }

    if (input->isPushed(HG::Core::Input::Keyboard::Key::F))
    {
        if (m_camera->projection() == HG::Rendering::Base::Camera::Projection::Orthogonal)
        {
            m_camera->setProjection(HG::Rendering::Base::Camera::Projection::Perspective);
        }
        else
        {
            m_camera->setProjection(HG::Rendering::Base::Camera::Projection::Orthogonal);
        }
    }

    gameObject()->transform()->setGlobalPosition(
        gameObject()->transform()->globalPosition() +
            inputDirection * m_camera->gameObject()->transform()->globalRotation()
    );
}
