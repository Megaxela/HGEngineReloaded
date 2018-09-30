// HG::Core
#include <Application.hpp>

// HG::Rendering::OpenGL
#include <Forward/AbstractRenderer.hpp>
#include <Common/Texture2DData.hpp>
#include <Common/ShaderData.hpp>

// HG::Rendering::Base
#include <Renderer.hpp>
#include <Material.hpp>
#include <Texture.hpp>
#include <Shader.hpp>
#include <CubeMap.hpp>

// GLM
#include <gl/auxiliary/glm_uniforms.hpp>
#include <Common/CubeMapTextureData.hpp>

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

    auto program = &static_cast<Common::ShaderData*>(material->shader()->specificData())->Program;

    for (auto&& [uniformName, uniformValue] : *material)
    {
        setShaderUniform(program, uniformName, uniformValue);
    }
}

void HG::Rendering::OpenGL::Forward::AbstractRenderer::setShaderUniform(gl::program *program,
                                                                           const std::string &name,
                                                                           const HG::Rendering::Base::MaterialValue &value)
{

    auto location = program->uniform_location(name);

    switch (value.type)
    {
    case Base::MaterialValue::Type::Int:
        program->set_uniform(
            location,
            value.integer
        );
        break;
    case Base::MaterialValue::Type::Float:
        program->set_uniform(
            location,
            value.floating
        );
        break;
    case Base::MaterialValue::Type::Boolean:
        program->set_uniform(
            location,
            value.boolean
        );
        break;
    case Base::MaterialValue::Type::Vector2:
        program->set_uniform(
            location,
            value.vector2
        );
        break;
    case Base::MaterialValue::Type::Vector3:
        program->set_uniform(
            location,
            value.vector3
        );
        break;
    case Base::MaterialValue::Type::Vector4:
        program->set_uniform(
            location,
            value.vector4
        );
        break;
    case Base::MaterialValue::Type::Matrix2x2:
        program->set_uniform(
            location,
            value.mat2x2
        );
        break;
    case Base::MaterialValue::Type::Matrix3x3:
        program->set_uniform(
            location,
            value.mat3x3
        );
        break;
    case Base::MaterialValue::Type::Matrix4x4:
        program->set_uniform(
            location,
            value.mat4x4
        );
        break;

    case Base::MaterialValue::Type::CubeMap:
    {
        // todo: If any errors on texture, render fallback texture.
        // Setting texture unit
        program->set_uniform_1i(
            location,
            m_textureNumber
        );

        auto cubemapData = static_cast<Common::CubeMapTextureData*>(value.cubeMap->specificData());

        // Setup texture if not valid
        if (application()->renderer()->needSetup(value.cubeMap))
        {
            if (!application()->renderer()->setup(value.cubeMap))
            {
                // FALLBACK HERE
                return;
            }

            cubemapData = static_cast<Common::CubeMapTextureData*>(value.cubeMap->specificData());
        }

        cubemapData->Texture.set_active(m_textureNumber);
        cubemapData->Texture.bind();

        ++m_textureNumber;

        break;
    }
    case Base::MaterialValue::Type::Texture:
    {
        // todo: If any errors on texture, render fallback texture.
        // Setting texture unit
        program->set_uniform_1i(
            location,
            m_textureNumber
        );

        auto textureData = static_cast<Common::Texture2DData*>(value.texture->specificData());

        // Setup texture if not valid
        if (application()->renderer()->needSetup(value.texture))
        {
            if (!application()->renderer()->setup(value.texture))
            {
                // FALLBACK HERE
                return;
            }

            textureData = static_cast<Common::Texture2DData*>(value.texture->specificData());
        }

        textureData->Texture.set_active(m_textureNumber);
        textureData->Texture.bind();

        ++m_textureNumber;

        break;
    }
    }
}