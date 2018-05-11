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

        /**
         * @brief Method for setting sens of mouse
         * movement. 3 by default.
         * @param sens Sensetivity.
         */
        void setSensitivity(float sens);

        /**
         * @brief Method for getting mouse sensitivity.
         * 3 by default.
         * @return sensitivity.
         */
        float sensitivity() const;

    protected:
        void onStart() override;

        void onUpdate() override;


    private:

        bool m_enabled;
        bool m_first;
        glm::vec2 m_lastMousePosition;
        glm::vec3 m_front;
        float m_yaw;
        float m_pitch;
        ::RENDERING_BASE_MODULE_NS::Camera* m_camera;

        float m_sensitivity;
    };
}

