
#include <Material.hpp>

#include "Material.hpp"

HG::Rendering::Base::Material::Material() :
    m_variableContainer(),
    m_shader(nullptr)
{

}

HG::Rendering::Base::Material::Material(HG::Rendering::Base::Shader *shader) :
    m_variableContainer(),
    m_shader(shader)
{

}

void HG::Rendering::Base::Material::set(const std::string& name, int value)
{
    Value val{};
    val.type = Value::Type::Int;
    val.integer = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, float value)
{
    Value val{};
    val.type = Value::Type::Float;
    val.floating = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, bool value)
{
    Value val{};
    val.type = Value::Type::Boolean;
    val.boolean = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::vec2 value)
{
    Value val{};
    val.type = Value::Type::Vector2;
    val.vector2 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::vec3 value)
{
    Value val{};
    val.type = Value::Type::Vector3;
    val.vector3 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::vec4 value)
{
    Value val{};
    val.type = Value::Type::Vector4;
    val.vector4 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::mat2 value)
{
    Value val{};
    val.type = Value::Type::Matrix2x2;
    val.mat2x2= value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::mat3 value)
{
    Value val{};
    val.type = Value::Type::Matrix3x3;
    val.mat3x3 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::mat4 value)
{
    Value val{};
    val.type = Value::Type::Matrix4x4;
    val.mat4x4 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, HG::Rendering::Base::Texture* value)
{
    Value val{};
    val.type = Value::Type::Texture;
    val.texture = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, HG::Rendering::Base::Material::Value value)
{
    m_variableContainer[name] = value;
}

void HG::Rendering::Base::Material::erase(const std::string& name)
{
    m_variableContainer.erase(name);
}

HG::Rendering::Base::Material::VariablesContainer::const_iterator
HG::Rendering::Base::Material::begin() const
{
    return m_variableContainer.begin();
}

HG::Rendering::Base::Material::VariablesContainer::const_iterator
HG::Rendering::Base::Material::end() const
{
    return m_variableContainer.end();
}

void HG::Rendering::Base::Material::setShader(HG::Rendering::Base::Shader *shader)
{
    m_shader = shader;
}

HG::Rendering::Base::Shader *HG::Rendering::Base::Material::shader() const
{
    return m_shader;
}
