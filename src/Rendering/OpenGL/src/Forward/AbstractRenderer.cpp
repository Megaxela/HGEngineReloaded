#include <Common/ShaderData.hpp>
#include <Common/Texture2DData.hpp>
#include "Forward/AbstractRenderer.hpp"
#include <gl/auxiliary/glm_uniforms.hpp>

HG::Rendering::OpenGL::Forward::AbstractRenderer::AbstractRenderer() :
    m_application(nullptr),
    m_textureNumber(0)
{

}

HG::Core::Application *HG::Rendering::OpenGL::Forward::AbstractRenderer::application() const
{
    return m_application;
}

void HG::Rendering::OpenGL::Forward::AbstractRenderer::setApplication(HG::Core::Application *application)
{
    m_application = application;
}

void HG::Rendering::OpenGL::Forward::AbstractRenderer::applyShaderUniforms(HG::Rendering::Base::Material *material)
{
    m_textureNumber = 0;

    auto program = &material->shader()->externalData<Common::ShaderData>()->Program;

    for (auto&& [uniformName, uniformValue] : *material)
    {
        setShaderUniform(program, uniformName, uniformValue);
    }
}

void HG::Rendering::OpenGL::Forward::AbstractRenderer::setShaderUniform(gl::program *program,
                                                                           const std::string &name,
                                                                           const HG::Rendering::Base::Material::Value &value)
{

    auto location = program->uniform_location(name);

    switch (value.type)
    {
    case Base::Material::Value::Type::Int:
        program->set_uniform(
            location,
            value.integer
        );
        break;
    case Base::Material::Value::Type::Float:
        program->set_uniform(
            location,
            value.floating
        );
        break;
    case Base::Material::Value::Type::Boolean:
        program->set_uniform(
            location,
            value.boolean
        );
        break;
    case Base::Material::Value::Type::Vector2:
        program->set_uniform(
            location,
            value.vector2
        );
        break;
    case Base::Material::Value::Type::Vector3:
        program->set_uniform(
            location,
            value.vector3
        );
        break;
    case Base::Material::Value::Type::Vector4:
        program->set_uniform(
            location,
            value.vector4
        );
        break;
    case Base::Material::Value::Type::Matrix2x2:
        program->set_uniform(
            location,
            value.mat2x2
        );
        break;
    case Base::Material::Value::Type::Matrix3x3:
        program->set_uniform(
            location,
            value.mat3x3
        );
        break;
    case Base::Material::Value::Type::Matrix4x4:
        program->set_uniform(
            location,
            value.mat4x4
        );
        break;
    case Base::Material::Value::Type::Texture:
        // todo: If any errors on texture, render fallback texture.
        // Setting texture unit
        program->set_uniform_1i(
            location,
            m_textureNumber
        );

        value.texture->externalData<Common::Texture2DData>()->Texture.set_active(m_textureNumber);
        value.texture->externalData<Common::Texture2DData>()->Texture.bind();

        ++m_textureNumber;

        break;
    }
}