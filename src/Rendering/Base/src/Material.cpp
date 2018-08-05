
#include <Material.hpp>

#include "Material.hpp"

RENDERING_BASE_MODULE_NS::Material::Material() :
    m_variableContainer(),
    m_shader(nullptr)
{

}

RENDERING_BASE_MODULE_NS::Material::Material(RENDERING_BASE_MODULE_NS::Shader *shader) :
    m_variableContainer(),
    m_shader(shader)
{

}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, int value)
{
    Value val{};
    val.type = Value::Type::Int;
    val.integer = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, float value)
{
    Value val{};
    val.type = Value::Type::Float;
    val.floating = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, bool value)
{
    Value val{};
    val.type = Value::Type::Boolean;
    val.boolean = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, glm::vec2 value)
{
    Value val{};
    val.type = Value::Type::Vector2;
    val.vector2 = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, glm::vec3 value)
{
    Value val{};
    val.type = Value::Type::Vector3;
    val.vector3 = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, glm::vec4 value)
{
    Value val{};
    val.type = Value::Type::Vector4;
    val.vector4 = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, glm::mat2 value)
{
    Value val{};
    val.type = Value::Type::Matrix2x2;
    val.mat2x2= value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, glm::mat3 value)
{
    Value val{};
    val.type = Value::Type::Matrix3x3;
    val.mat3x3 = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, glm::mat4 value)
{
    Value val{};
    val.type = Value::Type::Matrix4x4;
    val.mat4x4 = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, RENDERING_BASE_MODULE_NS::Texture* value)
{
    Value val{};
    val.type = Value::Type::Texture;
    val.texture = value;

    set(name, val);
}

void RENDERING_BASE_MODULE_NS::Material::set(const std::string& name, RENDERING_BASE_MODULE_NS::Material::Value value)
{
    m_variableContainer[name] = value;
}

void RENDERING_BASE_MODULE_NS::Material::erase(const std::string& name)
{
    m_variableContainer.erase(name);
}

RENDERING_BASE_MODULE_NS::Material::VariablesContainer::const_iterator
RENDERING_BASE_MODULE_NS::Material::begin() const
{
    return m_variableContainer.begin();
}

RENDERING_BASE_MODULE_NS::Material::VariablesContainer::const_iterator
RENDERING_BASE_MODULE_NS::Material::end() const
{
    return m_variableContainer.end();
}

void RENDERING_BASE_MODULE_NS::Material::setShader(RENDERING_BASE_MODULE_NS::Shader *shader)
{
    m_shader = shader;
}

RENDERING_BASE_MODULE_NS::Shader *RENDERING_BASE_MODULE_NS::Material::shader() const
{
    return m_shader;
}
