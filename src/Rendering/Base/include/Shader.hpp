#pragma once

#include <utility>
#include <string>

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes shader.
     */
    class Shader
    {
    public:

        /**
         * @brief Class, that describes abstract
         * external data for shader.
         */
        class ShaderExternalData
        {
        public:

            /**
             * @brief Default virtual destructor.
             */
            virtual ~ShaderExternalData() = default;
        };

        /**
         * @brief Constructor.
         */
        Shader();

        /**
         * @brief Destructor.
         */
        ~Shader();

        /**
         * @brief Method for getting external data, casted
         * to some type.
         * @tparam T Type to cast.
         * @return Pointer to external data.
         */
        template<typename T>
        typename std::enable_if<
                std::is_base_of<ShaderExternalData, T>::value,
                T*
        >::type externalData() const
        {
            return static_cast<T*>(m_externalData);
        }

        /**
         * @brief Method for setting external data.
         * @tparam T Type of actual external data.
         */
        template<typename T>
        typename std::enable_if<
                std::is_base_of<ShaderExternalData, T>::value
        >::type setExternalData()
        {
            delete m_externalData;

            m_externalData = new T();
        }

        /**
         * @brief Method for clearing external data.
         */
        void clearExternalData();

        /**
         * @brief Method for setting shader text.
         * Format:
         *
         * ```glsl
         * #ifdef VertexShader
         * layout (location = 0) in vec3 inPosition;  // Vertex position
         * layout (location = 1) in vec3 inNormals;   // Normal vector
         * layout (location = 2) in vec2 inTexCoords; // Texture coords
         *
         * uniform mat4 model; // Model matrix
         * uniform mat4 view; // View matrix
         * uniform mat4 projection; // Projection matrix
         *
         * // Vertex shader here
         * #endif
         *
         * #ifdef FragmentShader
         * // Fragment shader here
         * #endif
         * ```
         *
         * Internal constants:
         * MAX_POINT_LIGHTS - maximum internal number of point lights.
         * MAX_DIRECTIONAL_LIGHTS - maximum internal number of directional lights.
         * MAX_SPOT_LIGHTS - maximum internal number of spot lights.
         *
         * Internal structures:
         * ```glsl
         *
         * struct PointLight
         * {
         *     vec3 position;
         *
         *     float constant;
         *     float linear;
         *     float quadratic;
         *
         *     vec3 ambient;
         *     vec3 diffuse;
         *     vec3 specular;
         * }
         *
         * struct DirectionalLight
         * {
         *     vec3 direction;
         *
         *     vec3 ambient;
         *     vec3 diffuse;
         *     vec3 specular;
         * }
         *
         * struct SpotLight
         * {
         *     vec3 position;
         *     vec3 direction;
         *     float cutOff;
         *     float outerCutOff;
         *
         *     float constant;
         *     float linear;
         *     float quadratic;
         *
         *     vec3 ambient;
         *     vec3 diffuse;
         *     vec3 specular;
         * }
         * ```
         *
         * Available `layouts`:
         * 0 - vec3 - Vertex position;
         * 1 - vec3 - Normal vector;
         * 2 - vec2 - Texture coords;
         * 3 - vec3 - Tangent coords;
         * 4 - vec3 - Bitangent coords;
         *
         * Available `uniforms`:
         * mat4 - model - Model matrix;
         * mat4 - view - View matrix;
         * mat4 - projection - Projection matrix;
         * PointLight[MAX_POINT_LIGHTS] - pointLights - Array with point lights in scene.
         * int - numberOfPointLights - real amount of point lights in scene.
         * DirectionalLight[MAX_DIRECTIONAL_LIGHTS] - directionalLights - Array with directional lights in scene.
         * int - numberOfDirectionalLights - real amount of directional lights in scene.
         * SpotLight[MAX_SPOT_LIGHTS] - spotLights - Array with spot lights in scene.
         * int - numberOfSpotLights - real amount of spot lights in scene.
         * @param text Shader text.
         */
        void setShaderText(std::string text);

        /**
         * @brief Method for getting shader text.
         * @return Shader text.
         */
        std::string shaderText() const;

    private:

        std::string m_shaderText;

        ShaderExternalData* m_externalData;
    };
}


