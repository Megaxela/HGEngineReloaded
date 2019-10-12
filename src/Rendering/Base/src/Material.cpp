// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

namespace HG::Rendering::Base
{
Material::Material() : m_variableContainer(), m_shader(nullptr)
{
}

Material::Material(const Material& rhs) : m_variableContainer(rhs.m_variableContainer), m_shader(rhs.m_shader)
{
}

Material& Material::operator=(const Material& rhs)
{
    m_variableContainer = rhs.m_variableContainer;
    m_shader            = rhs.m_shader;

    return (*this);
}

Material::Material(Shader* shader) : m_variableContainer(), m_shader(shader)
{
}

void Material::set(const std::string& name, int value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Int;
    val.integer = value;

    set(name, val);
}

void Material::set(const std::string& name, float value)
{
    MaterialValue val{};
    val.type     = MaterialValue::Type::Float;
    val.floating = value;

    set(name, val);
}

void Material::set(const std::string& name, bool value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Boolean;
    val.boolean = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::vec2 value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Vector2;
    val.vector2 = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::vec3 value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Vector3;
    val.vector3 = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::vec4 value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Vector4;
    val.vector4 = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::mat2 value)
{
    MaterialValue val{};
    val.type   = MaterialValue::Type::Matrix2x2;
    val.mat2x2 = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::mat3 value)
{
    MaterialValue val{};
    val.type   = MaterialValue::Type::Matrix3x3;
    val.mat3x3 = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::mat4 value)
{
    MaterialValue val{};
    val.type   = MaterialValue::Type::Matrix4x4;
    val.mat4x4 = value;

    set(name, val);
}

void Material::set(const std::string& name, Texture* value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Texture;
    val.texture = value;

    set(name, val);
}

void Material::set(const std::string& name, CubeMap* value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::CubeMap;
    val.cubeMap = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::ivec2 value)
{
    MaterialValue val{};
    val.type        = MaterialValue::Type::IntVector2;
    val.vector2_int = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::ivec3 value)
{
    MaterialValue val{};
    val.type        = MaterialValue::Type::IntVector3;
    val.vector3_int = value;

    set(name, val);
}

void Material::set(const std::string& name, glm::ivec4 value)
{
    MaterialValue val{};
    val.type        = MaterialValue::Type::IntVector4;
    val.vector4_int = value;

    set(name, val);
}

void Material::set(const std::string& name, MaterialValue value)
{
    m_variableContainer[name] = value;
}

void Material::erase(const std::string& name)
{
    m_variableContainer.erase(name);
}

const Material::VariablesContainer& Material::uniformVaues() const
{
    return m_variableContainer;
}

void Material::setShader(Shader* shader)
{
    m_shader = shader;
}

Shader* Material::shader() const
{
    return m_shader;
}
} // namespace HG::Rendering::Base
