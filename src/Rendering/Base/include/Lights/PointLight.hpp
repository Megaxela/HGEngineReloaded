#pragma once

#include "AbstractLight.hpp"

namespace RENDERING_BASE_MODULE_NS::Lights
{
    /**
     * @brief Class, that describes point light.
     */
    class PointLight : public AbstractLight
    {
    public:

        /**
         * @brief Constructor.
         */
        PointLight();

        /**
         * @brief Method for getting point light
         * radius.
         * @return Radius.
         */
        float radius() const;

        /**
         * @brief Method for setting point light
         * radius.
         * @param rad Radius.
         */
        void setRadius(float rad);

    private:

        float m_radius;
    };
}


