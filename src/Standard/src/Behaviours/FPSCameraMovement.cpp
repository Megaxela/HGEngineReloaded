#include "Behaviours/FPSCameraMovement.hpp"

HG::Standard::Behaviours::FPSCameraMovement::FPSCameraMovement() :
    m_enabled(true),
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
    m_camera = gameObject()->findBehaviour<::RENDERING_BASE_MODULE_NS::Camera>();

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

    if (scene()->application()->input()->keyboard()->isPushed(
        ::CORE_MODULE_NS::Input::Keyboard::Key::R
    ))
    {
        m_enabled = !m_enabled;
    }

    if (m_enabled)
    {
        // Keyboard
        float keyboardSens = (float) (0.00005f * scene()->application()->timeStatistics()->frameDeltaTime().count());
//        if (Input::Keyboard::isPressed(Input::Keyboard::Key ::ArrowLeft))
//        {
//            m_yaw -= keyboardSens;
//        }
//        if (Input::Keyboard::isPressed(Input::Keyboard::Key::ArrowRight))
//        {
//            m_yaw += keyboardSens;
//        }
//        if (Input::Keyboard::isPressed(Input::Keyboard::Key::ArrowUp))
//        {
//            m_pitch -= keyboardSens;
//        }
//        if (Input::Keyboard::isPressed(Input::Keyboard::Key::ArrowDown))
//        {
//            m_pitch += keyboardSens;
//        }

        float mouseSens = 0.2f;

        // Mouse
        auto mousePosition = scene()->application()->input()->mouse()->getMousePosition();
        mousePosition -= m_lastMousePosition;

        m_yaw += mousePosition.x * mouseSens;
        m_pitch += mousePosition.y * mouseSens;

        m_lastMousePosition = scene()->application()->input()->mouse()->getMousePosition();

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
}
