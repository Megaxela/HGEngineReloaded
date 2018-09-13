#pragma once

// HG::Core
#include <Behaviour.hpp> // Required for inheritance

// GLM
#include <glm/glm.hpp>

namespace HG::Rendering::Base
{
    class Camera;
}

namespace HG::Standard::Behaviours
{
    /**
     * @brief Standard camera fps movement. Just moving
     * camera with mouse.
     */
    class FPSCameraMovement : public HG::Core::Behaviour
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
        HG::Rendering::Base::Camera* m_camera;

        HG_PROPERTY_DEFAULT(float, Sensitivity, 3.0f);
        HG_PROPERTY_DEFAULT(float, MovementSpeed, 2.0f);
    };
}

