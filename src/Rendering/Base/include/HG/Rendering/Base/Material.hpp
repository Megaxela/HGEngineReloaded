#pragma once

// C++ STL
#include <string>
#include <unordered_map>

// HG::Rendering::Base
#include <HG/Rendering/Base/MaterialValue.hpp>

namespace HG::Rendering::Base
{
    class Shader;

    /**
     * @brief Class, that describes material.
     * That will be used by renderer.
     */
    class Material
    {
    public:

        using VariablesContainer = std::unordered_map<std::string, MaterialValue>;

        /**
         * @brief Constructor.
         */
        Material();

        // todo: Implement move operators/constructors
        /**
         * @brief Copy constructor.
         */
        Material(const Material& rhs);

        /**
         * @brief Copy operator.
         */
        Material& operator=(const Material& rhs);

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
        void set(const std::string& name, HG::Rendering::Base::Texture* value);
        void set(const std::string& name, HG::Rendering::Base::CubeMap* value);

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
        void setShader(HG::Rendering::Base::Shader* shader);

        /**
         * @brief Method for getting material shader.
         * @return Pointer to shader.
         */
        HG::Rendering::Base::Shader* shader() const;

    protected:
        /**
         * @brief Method, that's called from `set` methods
         * after Value was formed.
         * @param name Name.
         * @param value Value.
         */
        void set(const std::string& name, MaterialValue value);

    private:
        VariablesContainer m_variableContainer;

        HG::Rendering::Base::Shader* m_shader;
    };
}

