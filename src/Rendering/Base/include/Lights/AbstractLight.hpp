#pragma once

#include <Color.hpp>
#include <Behaviour.hpp>
#include <Camera.hpp>

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes
     * abstract light.
     */
    class AbstractLight : public CORE_MODULE_NS::Behaviour
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
         * @brief Method for setting specular color.
         * @param color Specular color.
         */
        void setSpecular(const UTILS_MODULE_NS::Color &color);

        /**
         * @brief Method for getting light specular color.
         * @return Specular color.
         */
        UTILS_MODULE_NS::Color specular() const;

        /**
         * @brief Method for stting light ambient color.
         * @param color Ambient color.
         */
        void setAmbient(const UTILS_MODULE_NS::Color &color);

        /**
         * @brief Method for getting light ambient color.
         * @return Ambient color.
         */
        UTILS_MODULE_NS::Color ambient() const;

        /**
         * @brief Method for setting light diffuse color.
         * @param color Diffuse color.
         */
        void setDiffuse(const UTILS_MODULE_NS::Color &color);

        /**
         * @brief Method for getting light diffuse color.
         * @return Diffuse color.
         */
        UTILS_MODULE_NS::Color diffuse() const;

    protected:
        void onStart() override;

    private:
        static std::vector<AbstractLight*> m_lights;

        Type m_type;
        UTILS_MODULE_NS::Color m_specular;
        UTILS_MODULE_NS::Color m_ambient;
        UTILS_MODULE_NS::Color m_diffuse;
    };
}

