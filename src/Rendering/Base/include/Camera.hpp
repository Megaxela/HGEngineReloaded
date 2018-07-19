#pragma once

#include <glm/glm.hpp>
#include <Rect.hpp>
#include "Behaviour.hpp"

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes
     * gameobject camera object behaviour.
     * It's implemented as behaviour just
     * because it must be part of scene.
     */
    class Camera : public HG::Core::Behaviour
    {
    public:

        using CullType = float;

        /**
         * @brief Class that desctibes camera
         * orthogonal settings.
         */
        class OrthogonalSettings
        {
            friend class Camera;
        public:
            /**
             * @brief Constructor.
             */
            OrthogonalSettings();

            /**
             * @brief Copy constructor. It will ignore m_parentCam
             * field, because it's camera dependent.
             * @param rhs
             */
            OrthogonalSettings(const OrthogonalSettings& rhs);

            /**
             * @brief Method for setting zoom.
             * @param zoom Zoom value.
             */
            void setZoom(float zoom);

            /**
             * @brief Method for getting zoom value.
             * @return Zoom value.
             */
            float zoom() const;

            /**
             * @brief Method for setting orthogonal size.
             * @param size
             */
            void setSize(uint32_t size);

            /**
             * @brief Method for getting orthogonal size.
             * @return
             */
            uint32_t size() const;

            /**
             * @brief Set operator.
             * @param rhs Right hand object.
             */
            OrthogonalSettings& operator=(const OrthogonalSettings& rhs);

        private:
            float m_zoom;
            uint32_t m_size;

            Camera* m_parentCam;
        };

        /**
         * @brief Class, that describes camera
         * perspective settings.
         */
        class PerspectiveSettings
        {
            friend class Camera;
        public:
            /**
             * @brief Constructor.
             */
            PerspectiveSettings();

            /**
             * @brief Copy constructor.
             * @param rhs Right hand object.
             */
            PerspectiveSettings(const PerspectiveSettings& rhs);

            /**
             * @brief Method for setting camera
             * field of view. In degrees.
             * @param fov Field of view.
             */
            void setFieldOfView(float fov);

            /**
             * @brief Method for getting camera
             * field of view. In degrees.
             * @return Field of view value.
             */
            float fieldOfView() const;

            /**
             * @brief Set operator.
             */
            PerspectiveSettings& operator=(const PerspectiveSettings &rhs);

        private:
            float m_fov;

            Camera* m_parentCam;
        };

        /**
         * @brief Enum with projections.
         */
        enum class Projection
        {
            Perspective,
            Orthogonal
        };

        /**
         * @brief Method for getting currently
         * active camera. Or `nullptr` if there
         * is no such.
         * @return Pointer to camera.
         */
        static Camera* active();

        /**
         * @brief Method for getting all cameras
         * on scene.
         * @return Const reference to container.
         */
        static const std::vector<Camera*>& allCameras();

        /**
         * @brief Number of all cameras on scene.
         */
        static std::size_t allCamerasCount();

        /**
         * @brief Constructor.
         */
        Camera();

        /**
         * @brief Destructor.
         */
        ~Camera() override;

        /**
         * @brief Method for setting current projection.
         * Orthogonal or perspective.
         * @param projection Projection value.
         */
        void setProjection(Projection projection);

        /**
         * @brief Method for getting current camera
         * projection.
         * @return Current projection value.
         */
        Projection projection() const;

        /**
         * @brief Method for getting orthogonal settings
         * object.You can use it to define some orthogonal
         * specified settings for camera. Made with reference
         * for set operator.
         * @return Orthogonal settings object.
         */
        OrthogonalSettings& orthogonalSettings();

        /**
         * @brief Method for getting perspective settings
         * object. You can use it to define some perspective
         * specified settings for camera. Made with reference
         * for set operator.
         * @return
         */
        PerspectiveSettings& perspectiveSettings();

        /**
         * @brief Method for getting projection matrix
         * for this camera.
         * @return Matrix 4x4
         */
        glm::mat4 projectionMatrix() const;

        /**
         * @brief Method to get view matrix.
         * @return Matrix 4x4
         */
        glm::mat4 viewMatrix() const;

        /**
         * @brief Method for setting viewport rect.
         * @param x Left side position.
         * @param y Top side position.
         * @param w Width.
         * @param h Height.
         */
        void setViewport(int x, int y, int w, int h);

        /**
         * @brief Method for setting viewport.
         * @param rect Viewport rectangle.
         */
        void setViewport(const HG::Utils::Rect &rect);

        /**
         * @brief Method for getting viewport rect.
         * @return Viewport rect.
         */
        HG::Utils::Rect viewport() const;

        /**
         * @brief Method to set near value.
         * @param value Near value.
         */
        void setNear(CullType value);

        /**
         * @brief Method to get near value.
         * @return Near value
         */
        CullType getNear() const;

        /**
         * @brief Method to set far value.
         * @param value Far value.
         */
        void setFar(CullType value);

        /**
         * @brief Method to get far value.
         * @return Far value.
         */
        CullType getFar() const;

        /**
         * @brief Method for rotating gameobject
         * towards point.
         * @param point Point position.
         */
        void lookAt(const glm::vec3& point);

        /**
         * @brief Method for rotating gameobject
         * towards point with changed up vector.
         * @param point Point position.
         * @param upVector Up vector.
         */
        void lookAt(const glm::vec3& point, const glm::vec3& upVector);

    protected:
        void onStart() override;

    private:

        static Camera* m_active;

        static std::vector<Camera*> m_cameras;

        // Caching of projection matrix.
        mutable bool m_projectionMatrixChanged;
        mutable glm::mat4 m_projectionMatrix;

        Projection m_projection;

        HG::Utils::Rect m_viewport;

        CullType m_near;
        CullType m_far;

        OrthogonalSettings m_orthogonalSettings;
        PerspectiveSettings m_perspectiveSettings;
    };
}

