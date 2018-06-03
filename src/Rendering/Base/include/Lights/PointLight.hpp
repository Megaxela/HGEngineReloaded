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
         * @brief Method for setting constant radius.
         * @param value Value.
         */
        void setConstant(float value);

        /**
         * @brief Method for getting constant radius.
         * @return Value.
         */
        float constant() const;

        /**
         * @brief Method for setting linear radius.
         * @param value Value.
         */
        void setLinear(float value);

        /**
         * @brief Method for getting linear radius.
         * @return Value.
         */
        float linear() const;

        /**
         * @brief Method for setting quadratic radius.
         * @param value Value.
         */
        void setQuadratic(float value);

        /**
         * @brief Method for getting quadratic radius.
         * @return Value.
         */
        float quadratic() const;

    private:

        float m_constant;
        float m_linear;
        float m_quadratic;
    };
}


