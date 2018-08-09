#pragma once

#include <string>
#include <map>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "Texture.hpp"
#include "Shader.hpp"

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes material.
     * That will be used by renderer.
     */
    class Material
    {
    public:

        /**
         * @brief Struct, that describes
         * material variable value.
         */
        struct Value
        {
            enum class Type
            {
                Int,
                Float,
                Boolean,
                Vector2,
                Vector3,
                Vector4,
                Matrix2x2,
                Matrix3x3,
                Matrix4x4,
                Texture,
            };

            Type type;

            union {
                int integer;
                float floating;
                bool boolean;
                glm::vec2 vector2;
                glm::vec3 vector3;
                glm::vec4 vector4;
                glm::mat2 mat2x2;
                glm::mat3 mat3x3;
                glm::mat4 mat4x4;
                Texture* texture;
            };
        };

        using VariablesContainer = std::map<std::string, Value>;

        /**
         * @brief Constructor.
         */
        Material();

        /**
         * @brief Initialization constructor.
         * @param shader Pointer to shader.
         */
        explicit Material(Shader* shader);

        /**
         * @brief
         */
        virtual ~Material() = default;

        // Different type setters
        void set(const std::string& name, int value);
        void set(const std::string& name, float value);
        void set(const std::string& name, bool value);
        void set(const std::string& name, glm::vec2 value);
        void set(const std::string& name, glm::vec3 value);
        void set(const std::string& name, glm::vec4 value);
        void set(const std::string& name, glm::mat2 value);
        void set(const std::string& name, glm::mat3 value);
        void set(const std::string& name, glm::mat4 value);
        void set(const std::string& name, Texture* value);

        void erase(const std::string& name);

        // todo: At C++20 change this to generator
        /**
         * @brief Method for getting material variables
         * begin iterator.
         * @return Constant iterator.
         */
        VariablesContainer::const_iterator begin() const;

        /**
         * @brief Method for getting material variables
         * end iterator.
         * @return Constant iterator.
         */
        VariablesContainer::const_iterator end() const;

        /**
         * @brief Method for setting shader to material.
         * @param shader Pointer to shader.
         */
        void setShader(Shader* shader);

        /**
         * @brief Method for getting material shader.
         * @return Pointer to shader.
         */
        Shader* shader() const;

    protected:
        /**
         * @brief Method, that's called from `set` methods
         * after Value was formed.
         * @param name Name.
         * @param value Value.
         */
        void set(const std::string& name, Value value);

    private:
        VariablesContainer m_variableContainer;

        Shader* m_shader;
    };
}

