// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

HG::Rendering::Base::Material::Material() : m_variableContainer(), m_shader(nullptr)
{
}

HG::Rendering::Base::Material::Material(const HG::Rendering::Base::Material& rhs) :
    m_variableContainer(rhs.m_variableContainer),
    m_shader(rhs.m_shader)
{
}

HG::Rendering::Base::Material& HG::Rendering::Base::Material::operator=(const HG::Rendering::Base::Material& rhs)
{
    m_variableContainer = rhs.m_variableContainer;
    m_shader            = rhs.m_shader;

    return (*this);
}

HG::Rendering::Base::Material::Material(HG::Rendering::Base::Shader* shader) : m_variableContainer(), m_shader(shader)
{
}

void HG::Rendering::Base::Material::set(const std::string& name, int value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Int;
    val.integer = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, float value)
{
    MaterialValue val{};
    val.type     = MaterialValue::Type::Float;
    val.floating = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, bool value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Boolean;
    val.boolean = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::vec2 value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Vector2;
    val.vector2 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::vec3 value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Vector3;
    val.vector3 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::vec4 value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Vector4;
    val.vector4 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::mat2 value)
{
    MaterialValue val{};
    val.type   = MaterialValue::Type::Matrix2x2;
    val.mat2x2 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::mat3 value)
{
    MaterialValue val{};
    val.type   = MaterialValue::Type::Matrix3x3;
    val.mat3x3 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::mat4 value)
{
    MaterialValue val{};
    val.type   = MaterialValue::Type::Matrix4x4;
    val.mat4x4 = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, HG::Rendering::Base::Texture* value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::Texture;
    val.texture = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, HG::Rendering::Base::CubeMap* value)
{
    MaterialValue val{};
    val.type    = MaterialValue::Type::CubeMap;
    val.cubeMap = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::ivec2 value)
{
    MaterialValue val{};
    val.type        = MaterialValue::Type::IntVector2;
    val.vector2_int = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::ivec3 value)
{
    MaterialValue val{};
    val.type        = MaterialValue::Type::IntVector3;
    val.vector3_int = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, glm::ivec4 value)
{
    MaterialValue val{};
    val.type        = MaterialValue::Type::IntVector4;
    val.vector4_int = value;

    set(name, val);
}

void HG::Rendering::Base::Material::set(const std::string& name, HG::Rendering::Base::MaterialValue value)
{
    m_variableContainer[name] = value;
}

void HG::Rendering::Base::Material::erase(const std::string& name)
{
    m_variableContainer.erase(name);
}

const HG::Rendering::Base::Material::VariablesContainer& HG::Rendering::Base::Material::uniformVaues() const
{
    return m_variableContainer;
}

void HG::Rendering::Base::Material::setShader(HG::Rendering::Base::Shader* shader)
{
    m_shader = shader;
}

HG::Rendering::Base::Shader* HG::Rendering::Base::Material::shader() const
{
    return m_shader;
}
