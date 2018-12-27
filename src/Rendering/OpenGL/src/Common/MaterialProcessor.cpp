// HG::Core
#include <HG/Core/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/Base/CubeMap.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/Shader.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/CubeMapTextureData.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DData.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Common/MaterialProcessor.hpp>
#include <HG/Core/Application.hpp>

// GLM
#include <gl/auxiliary/glm_uniforms.hpp>

HG::Rendering::OpenGL::Common::MaterialProcessor::MaterialProcessor()
{

}

void HG::Rendering::OpenGL::Common::MaterialProcessor::useMaterial(HG::Core::Application* application, HG::Rendering::Base::Material* material)
{
    // todo: Replace static with global rendering state
    static gl::program* current = nullptr;

    if (application->renderer()->needSetup(material->shader()))
    {
        if (!application->renderer()->setup(material->shader(), true))
        {
            // If can't setup shader
            return;
        }
    }

    auto* matProgram =
        &material
            ->shader()
            ->castSpecificDataTo<Common::ShaderData>()->Program;

    if (current != matProgram)
    {
        current = matProgram;

        current->use();
    }
}

void HG::Rendering::OpenGL::Common::MaterialProcessor::applyMaterialUniforms(HG::Core::Application* application, HG::Rendering::Base::Material* material, bool guarantee)
{
    uint32_t textureNumber = 0;

    auto* shaderData = material->shader()->castSpecificDataTo<Common::ShaderData>();

    for (auto&& [uniformName, uniformValue] : material->uniformVaues())
    {
        setShaderUniform(application, shaderData, uniformName, uniformValue, textureNumber, guarantee);
    }
}

void HG::Rendering::OpenGL::Common::MaterialProcessor::setShaderUniform(
        HG::Core::Application* application,
        HG::Rendering::OpenGL::Common::ShaderData *shaderData,
        const std::string &name,
        const HG::Rendering::Base::MaterialValue &value,
        uint32_t &textureNumber,
        bool guarantee)
{
    // Trying to get cached uniform location
    auto locationIter = shaderData->UniformLocations.find(name);

    // If we fail - perform uniform_location query, store it and continue
    if (locationIter == shaderData->UniformLocations.end())
    {
        auto location = shaderData->Program.uniform_location(name);

        locationIter = shaderData->UniformLocations.insert({name, location}).first;
    }

    // If it's -1 - then there is no such uniform. Skip it.
    if (locationIter->second == -1)
    {
        return;
    }

    // Checking value hash
    auto hash = std::hash<HG::Rendering::Base::MaterialValue>()(value);

    auto savedHashIter = shaderData->CurrentUniformValueHashes.find(locationIter->second);

    bool skipSetting = false;

    if (savedHashIter != shaderData->CurrentUniformValueHashes.end())
    {
        // If it's same value - skip it
        if (savedHashIter->second == hash)
        {
            skipSetting = true;
        }
    }

    if (skipSetting)
    {
        switch (value.type)
        {
        case Base::MaterialValue::Type::Int:
        case Base::MaterialValue::Type::Float:
        case Base::MaterialValue::Type::Boolean:
        case Base::MaterialValue::Type::Vector2:
        case Base::MaterialValue::Type::Vector3:
        case Base::MaterialValue::Type::Vector4:
        case Base::MaterialValue::Type::IntVector2:
        case Base::MaterialValue::Type::IntVector3:
        case Base::MaterialValue::Type::IntVector4:
        case Base::MaterialValue::Type::Matrix2x2:
        case Base::MaterialValue::Type::Matrix3x3:
        case Base::MaterialValue::Type::Matrix4x4:
            return; // Skip this values

        case Base::MaterialValue::Type::CubeMap:
        case Base::MaterialValue::Type::Texture:
            break; // Continue

        default:
            throw std::invalid_argument("Unknown material value type");
        }
    }

    switch (value.type)
    {
    case Base::MaterialValue::Type::Int:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.integer
        );
        break;
    case Base::MaterialValue::Type::Float:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.floating
        );
        break;
    case Base::MaterialValue::Type::Boolean:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.boolean
        );
        break;
    case Base::MaterialValue::Type::Vector2:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.vector2
        );
        break;
    case Base::MaterialValue::Type::Vector3:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.vector3
        );
        break;
    case Base::MaterialValue::Type::Vector4:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.vector4
        );
        break;
    case Base::MaterialValue::Type::IntVector2:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.vector2_int
        );
        break;
    case Base::MaterialValue::Type::IntVector3:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.vector3_int
        );
        break;
    case Base::MaterialValue::Type::IntVector4:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.vector4_int
        );
        break;
    case Base::MaterialValue::Type::Matrix2x2:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.mat2x2
        );
        break;
    case Base::MaterialValue::Type::Matrix3x3:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.mat3x3
        );
        break;
    case Base::MaterialValue::Type::Matrix4x4:
        shaderData->Program.set_uniform(
            locationIter->second,
            value.mat4x4
        );
        break;

    case Base::MaterialValue::Type::CubeMap:
    {
        if (value.cubeMap == nullptr)
        {
            return;
        }

        // todo: If any errors on texture, render fallback texture.
        // Setting texture unit

        shaderData->Program.set_uniform_1i(
            locationIter->second,
            textureNumber
        );

        auto cubemapData = value.cubeMap->castSpecificDataTo<Common::CubeMapTextureData>();

        // Setup texture if not valid
        if (application->renderer()->needSetup(value.cubeMap))
        {
            if (!application->renderer()->setup(value.cubeMap, guarantee))
            {
                // FALLBACK HERE
                return;
            }

            cubemapData = value.cubeMap->castSpecificDataTo<Common::CubeMapTextureData>();
        }

        cubemapData->Texture.set_active(textureNumber);
        cubemapData->Texture.bind();

        ++textureNumber;

        break;
    }
    case Base::MaterialValue::Type::Texture:
    {
        if  (value.texture == nullptr)
        {
            return;
        }

        // todo: If any errors on texture, render fallback texture.
        // Setting texture unit

        shaderData->Program.set_uniform_1i(
            locationIter->second,
            textureNumber
        );

        auto textureData = value.texture->castSpecificDataTo<Common::Texture2DData>();

        // Setup texture if not valid
        if (application->renderer()->needSetup(value.texture))
        {
            if (guarantee)
            {
                Info() << "Loading texture with guarantee";
            }

            if (!application->renderer()->setup(value.texture, guarantee))
            {
                if (guarantee)
                {
                    Info() << "Can't load guaranteed texture";
                }
                // FALLBACK HERE
                return;
            }

            textureData = value.texture->castSpecificDataTo<Common::Texture2DData>();
        }

        textureData->Texture.set_active(textureNumber);
        textureData->Texture.bind();

        ++textureNumber;

        break;
    }
    }

    shaderData->CurrentUniformValueHashes.insert_or_assign(locationIter->second, hash);
}
