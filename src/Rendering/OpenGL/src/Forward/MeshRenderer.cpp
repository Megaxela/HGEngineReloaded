#include "Forward/MeshRenderer.hpp"
#include <Behaviours/Mesh.hpp>
#include <Materials/MeshFallbackMaterial.hpp>
#include <Camera.hpp>
#include <Lights/AbstractLight.hpp>
#include <Lights/PointLight.hpp>
#include <Lights/DirectionalLight.hpp>
#include <Common/MeshData.hpp>
#include <Common/ShaderData.hpp>
#include <Mesh.hpp>
#include <gl/auxiliary/glm_uniforms.hpp>

HG::Rendering::OpenGL::Forward::MeshRenderer::MeshRenderer() :
    m_meshFallbackMaterial(nullptr)
{

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

void HG::Rendering::OpenGL::Forward::MeshRenderer::render(HG::Core::GameObject *gameObject,
                                                            HG::Rendering::Base::RenderBehaviour *renderBehaviour)
{
    auto meshBehaviour = static_cast<HG::Rendering::Base::Behaviours::Mesh*>(renderBehaviour);
    
    auto data = meshBehaviour->externalData<Common::MeshData>();

    // todo: On errors, render "error" mesh instead.
    // Additional information in mesh is
    // contains VAO, VBO, EBO, so here is just
    // rendering of that info.
    if ( data == nullptr ||
         !data->VAO.is_valid() ||
         !data->VBO.is_valid() ||
         !data->EBO.is_valid())
    {
        Error()
            << "Mesh rendering behaviour of \""
            << meshBehaviour->gameObject()->name()
            << "\" has not external data or has non valid buffers.";
        return;
    }

    gl::program* program = nullptr;

    if (meshBehaviour->material() == nullptr ||
        meshBehaviour->material()->shader() == nullptr ||
        meshBehaviour->material()->shader()->externalData<Common::ShaderData>() == nullptr)
    {
        program = &m_meshFallbackMaterial->shader()->externalData<Common::ShaderData>()->Program;

        program->use();
    }
    else
    {
        program = &meshBehaviour->material()->shader()->externalData<Common::ShaderData>()->Program;

        program->use();

        applyShaderUniforms(meshBehaviour->material());
    }

    // Checking for VBO, VAO and EBO

    GLint location;

    if ((location = program->uniform_location("model")) != -1)
    {
        program->set_uniform(
            location,
            gameObject->transform()->localToWorldMatrix()
        );
    }

    if ((location = program->uniform_location("view")) != -1)
    {
        program->set_uniform(
            location,
            HG::Rendering::Base::Camera::active()->viewMatrix()
        );
    }

    if ((location = program->uniform_location("projection")) != -1)
    {
        program->set_uniform(location, HG::Rendering::Base::Camera::active()->projectionMatrix());
    }

    // Setting lighting uniforms
    auto& lights = HG::Rendering::Base::AbstractLight::totalLights();

    int pointLightIndex = 0;
    int directionalLightIndex = 0;
    int spotLightIndex = 0;

    for (auto&& light : lights)
    {
        switch (light->type())
        {
        case HG::Rendering::Base::AbstractLight::Type::Point:
        {
            auto castedLight = static_cast<HG::Rendering::Base::Lights::PointLight*>(light);

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].position")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->gameObject()->transform()->globalPosition()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].constant")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->constant()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].linear")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->linear()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].quadratic")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->quadratic()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].ambient")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->ambient().toRGBVector()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].diffuse")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->diffuse().toRGBVector()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].specular")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->specular().toRGBVector()
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
    if ((location = program->uniform_location("numberOfPointLights")) != -1)
    {
        program->set_uniform(location, pointLightIndex);
    }

    // Setting number of directional lights
    if ((location = program->uniform_location("numberOfDirectionalLights")) != -1)
    {
        program->set_uniform(location, directionalLightIndex);
    }

    // Setting number of spot lights
    if ((location = program->uniform_location("numberOfSpotLights")) != -1)
    {
        program->set_uniform(location, spotLightIndex);
    }

    // Setting camera position to shader
    if ((location = program->uniform_location("viewPos")) != -1)
    {
        program->set_uniform(
            location,
            HG::Rendering::Base::Camera::active()
                ->gameObject()
                ->transform()
                ->globalPosition()
        );
    }

    data->VAO.bind();

    gl::draw_range_elements(
        GL_TRIANGLES, // mode
        0,            // start
        static_cast<GLuint>(meshBehaviour->mesh()->Indices.size()),
        static_cast<GLsizei>(meshBehaviour->mesh()->Indices.size()),
        GL_UNSIGNED_INT
    );

    data->VAO.unbind();
}

size_t HG::Rendering::OpenGL::Forward::MeshRenderer::getTarget()
{
    return HG::Rendering::Base::Behaviours::Mesh::Id;
}
