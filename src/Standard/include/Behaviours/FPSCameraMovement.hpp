#pragma once

#include <Behaviour.hpp>
#include <Camera.hpp>

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Standard camera fps movement. Just moving
     * camera with mouse.
     */
    class FPSCameraMovement : public ::CORE_MODULE_NS::Behaviour
    {
    public:

        /**
         * @brief Default constructor.
         */
        FPSCameraMovement();

    protected:
        void onStart() override;

        void onUpdate() override;

    private:

        void handleKeyboardMovement();

        void handleMouseRotation();

        bool m_enabled;
        bool m_first;
        glm::vec2 m_lastMousePosition;
        glm::vec3 m_front;
        float m_yaw;
        float m_pitch;
        ::RENDERING_BASE_MODULE_NS::Camera* m_camera;

        HG_PROPERTY_DEFAULT(float, Sensitivity, 3.0f);
        HG_PROPERTY_DEFAULT(float, MovementSpeed, 2.0f);
    };
}

