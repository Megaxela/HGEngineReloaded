#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Lights/AbstractLight.hpp> // Required for inheritance

namespace HG::Rendering::Base::Lights
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
    HG_PROPERTY_INITIALIZER_RAW(Constant, float, setConstant, constant);
    HG_PROPERTY_INITIALIZER_RAW(Linear, float, setLinear, linear);
    HG_PROPERTY_INITIALIZER_RAW(Quadratic, float, setQuadratic, quadratic);

    float m_constant;
    float m_linear;
    float m_quadratic;
};
} // namespace HG::Rendering::Base::Lights
