// HG::Core
#include <Application.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

// HG::Rendering::OpenGL
#include <Materials/MeshFallbackMaterial.hpp>
#include <Forward/MeshRenderer.hpp>
#include <Common/ShaderData.hpp>
#include <Common/MeshData.hpp>

// HG::Rendering::Base
#include <Lights/DirectionalLight.hpp>
#include <Lights/AbstractLight.hpp>
#include <MaterialCollection.hpp>
#include <Lights/PointLight.hpp>
#include <Behaviours/Mesh.hpp>
#include <Renderer.hpp>
#include <Material.hpp>
#include <Camera.hpp>

// HG::Utils
#include <Mesh.hpp>

// GLM
#include <gl/auxiliary/glm_uniforms.hpp>

// gl
#include <gl/all.hpp>

HG::Rendering::OpenGL::Forward::MeshRenderer::MeshRenderer() :
    m_meshFallbackMaterial(nullptr)
{
    constexpr int count = 128;
    m_pointLightNames.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        m_pointLightNames.emplace_back(
            "pointLights[" + std::to_string(i) + "].position",
            "pointLights[" + std::to_string(i) + "].constant",
            "pointLights[" + std::to_string(i) + "].linear",
            "pointLights[" + std::to_string(i) + "].quadratic",
            "pointLights[" + std::to_string(i) + "].diffuse"
        );
    }
}

HG::Rendering::OpenGL::Forward::MeshRenderer::~MeshRenderer()
{
    deinit();
}

void HG::Rendering::OpenGL::Forward::MeshRenderer::init()
{
    Info() << "Initializing mesh renderer";

    m_meshFallbackMaterial = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<Materials::MeshFallbackMaterial>();
}

void HG::Rendering::OpenGL::Forward::MeshRenderer::deinit()
{
    Info() << "Deinitializing sprite renderer";

    delete m_meshFallbackMaterial;
    m_meshFallbackMaterial = nullptr;
}

void HG::Rendering::OpenGL::Forward::MeshRenderer::render(HG::Rendering::Base::RenderBehaviour *renderBehaviour)
{
    auto meshBehaviour = static_cast<HG::Rendering::Base::Behaviours::Mesh*>(renderBehaviour);
    auto data = static_cast<Common::MeshData*>(meshBehaviour->specificData());

    // todo: On errors, render "error" mesh instead.
    if (application()->renderer()->needSetup(meshBehaviour))
    {
        if (!application()->renderer()->setup(meshBehaviour))
        {
            return;
        }

        data = static_cast<Common::MeshData*>(meshBehaviour->specificData());
    }

    gl::program* program = nullptr;

    if (meshBehaviour->material() == nullptr ||
        meshBehaviour->material()->shader() == nullptr)
    {
        program = &static_cast<Common::ShaderData*>(m_meshFallbackMaterial->shader()->specificData())->Program;

        program->use();
    }
    else
    {
        Common::ShaderData* shaderData = nullptr;

        if (application()->renderer()->needSetup(meshBehaviour->material()->shader()))
        {
            if (!application()->renderer()->setup(meshBehaviour->material()->shader()))
            {
                // If can't setup shader
                shaderData = static_cast<Common::ShaderData*>(m_meshFallbackMaterial->shader()->specificData());
            }
        }

        if (shaderData == nullptr)
        {
            shaderData = static_cast<Common::ShaderData*>(meshBehaviour->material()->shader()->specificData());
        }

        program = &shaderData->Program;
        program->use();

        applyShaderUniforms(meshBehaviour->material());
    }

    // No active camera. No rendering.
    if (application()->renderer()->activeCamera() == nullptr)
    {
        return;
    }

    // Checking for VBO, VAO and EBO

    GLint location;

    static std::string modelName = "model";
    if ((location = program->uniform_location(modelName)) != -1)
    {
        program->set_uniform(
            location,
            meshBehaviour->gameObject()->transform()->localToWorldMatrix()
        );
    }

    static std::string viewName = "view";
    if ((location = program->uniform_location(viewName)) != -1)
    {
        program->set_uniform(
            location,
            application()->renderer()->activeCamera()->viewMatrix()
        );
    }

    static std::string projectionName = "projection";
    if ((location = program->uniform_location(projectionName)) != -1)
    {
        program->set_uniform(
            location,
            application()->renderer()->activeCamera()->projectionMatrix()
        );
    }

    // Setting lighting uniforms
    auto& lights = HG::Rendering::Base::AbstractLight::totalLights();

    int pointLightIndex = 0;
    int directionalLightIndex = 0;
    int spotLightIndex = 0;

    for (auto&& light : lights)
    {
        if ( light->gameObject() == nullptr ||
            !light->gameObject()->isEnabled() ||
            !light->isEnabled())
        {
            continue;
        }

        switch (light->type())
        {
        case HG::Rendering::Base::AbstractLight::Type::Point:
        {
            auto castedLight = static_cast<HG::Rendering::Base::Lights::PointLight*>(light);

            if ((location = program->uniform_location(m_pointLightNames[pointLightIndex].position)) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->gameObject()->transform()->globalPosition()
                );
            }

            if ((location = program->uniform_location(m_pointLightNames[pointLightIndex].constant)) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->constant()
                );
            }

            if ((location = program->uniform_location(m_pointLightNames[pointLightIndex].linear)) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->linear()
                );
            }

            if ((location = program->uniform_location(m_pointLightNames[pointLightIndex].quadratic)) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->quadratic()
                );
            }

            if ((location = program->uniform_location(m_pointLightNames[pointLightIndex].diffuse)) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->color().toRGBVector() * 300.0f
                );
            }

            ++pointLightIndex;
            break;
        }
        case HG::Rendering::Base::AbstractLight::Type::Directional:
        {
//            auto castedLight = static_cast<HG::Rendering::Base::Lights::DirectionalLight*>(light);

            // todo: Finish directional light uniform info
            ++directionalLightIndex;
            break;
        }
        case HG::Rendering::Base::AbstractLight::Type::Spot:
        {
//            auto castedLight = static_cast<HG::Rendering::Base::Lights::*>(light);

            // todo: Finish stop light uniform info
            ++spotLightIndex;
            break;
        }
        }
    }

    // todo: Add lighting caching
    // Setting number of point lights
    static std::string numberOfPointLightsName = "numberOfPointLights";
    if ((location = program->uniform_location(numberOfPointLightsName)) != -1)
    {
        program->set_uniform(location, pointLightIndex);
    }

    // Setting number of directional lights
    static std::string numberOfDirectionalLightsName = "numberOfDirectionalLights";
    if ((location = program->uniform_location(numberOfDirectionalLightsName)) != -1)
    {
        program->set_uniform(location, directionalLightIndex);
    }

    // Setting number of spot lights
    static std::string numberOfSpotLightsName = "numberOfSpotLights";
    if ((location = program->uniform_location(numberOfSpotLightsName)) != -1)
    {
        program->set_uniform(location, spotLightIndex);
    }

    // Setting camera position to shader
    static std::string viewPosName = "viewPos";
    if ((location = program->uniform_location(viewPosName)) != -1)
    {
        program->set_uniform(
            location,
            application()->renderer()->activeCamera()
                ->gameObject()
                ->transform()
                ->globalPosition()
        );
    }

    data->VAO.bind();

    gl::draw_range_elements(
        GL_TRIANGLES, // mode
        0,            // start
        static_cast<GLuint>(data->Count),
        static_cast<GLsizei>(data->Count),
        GL_UNSIGNED_INT
    );

    data->VAO.unbind();
}

size_t HG::Rendering::OpenGL::Forward::MeshRenderer::getTarget()
{
    return HG::Rendering::Base::Behaviours::Mesh::RenderBehaviourId;
}
