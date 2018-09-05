#pragma once

#include <Color.hpp>
#include <Behaviour.hpp>
#include <Camera.hpp>

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes
     * abstract light.
     */
    class AbstractLight : public HG::Core::Behaviour
    {
    public:

        /**
         * @brief Type of light.
         */
        enum class Type
        {
            Point,
            Directional,
            Spot
        };

        /**
         * @brief Constructor.
         */
        explicit AbstractLight(Type type);

        /**
         * @brief Virtual destructor.
         */
        ~AbstractLight() override;

        /**
         * @brief Method for getting light type.
         * @return Type enum class value.
         */
        Type type();

        /**
         * @brief Method for setting light diffuse color.
         * @param color Diffuse color.
         */
        void setColor(const HG::Utils::Color& color);

        /**
         * @brief Method for getting light diffuse color.
         * @return Diffuse color.
         */
        HG::Utils::Color color() const;

        /**
         * @brief Method for getting all lights on scene.
         * @return Constant reference on lights vector.
         */
        static const std::vector<AbstractLight*>& totalLights();

    protected:
        void onStart() override;

    private:
        static std::vector<AbstractLight*> m_lights;

        Type m_type;
        HG::Utils::Color m_color;

        HG_PROPERTY_INITIALIZER_RAW(Diffuse,  HG::Utils::Color, setColor,  color);
    };
}

