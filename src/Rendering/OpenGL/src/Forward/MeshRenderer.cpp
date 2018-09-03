#include <Forward/MeshRenderer.hpp>
#include <Materials/MeshFallbackMaterial.hpp>
#include <Lights/DirectionalLight.hpp>
#include <Lights/AbstractLight.hpp>
#include <Lights/PointLight.hpp>
#include <Behaviours/Mesh.hpp>
#include <Common/ShaderData.hpp>
#include <Common/MeshData.hpp>
#include <Application.hpp>
#include <GameObject.hpp>
#include <Material.hpp>
#include <Camera.hpp>
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

void HG::Rendering::OpenGL::Forward::MeshRenderer::render(HG::Rendering::Base::RenderBehaviour *renderBehaviour)
{
    auto meshBehaviour = static_cast<HG::Rendering::Base::Behaviours::Mesh*>(renderBehaviour);
    
    auto data = meshBehaviour->specificData<Common::MeshData>();

    // todo: On errors, render "error" mesh instead.
    // Additional information in mesh is
    // contains VAO, VBO, EBO, so here is just
    // rendering of that info.
    if ( data == nullptr ||
         !data->VAO.is_valid() ||
         !data->VBO.is_valid() ||
         !data->EBO.is_valid())
    {
        if (!application()->renderer()->setup(meshBehaviour))
        {
            Error()
                << "Mesh rendering behaviour of \""
                << meshBehaviour->gameObject()->name()
                << "\" has not external data or has non valid buffers.";
            return;
        }

        data = meshBehaviour->specificData<Common::MeshData>();
    }

    gl::program* program = nullptr;

    if (meshBehaviour->material() == nullptr ||
        meshBehaviour->material()->shader() == nullptr)
    {
        program = &m_meshFallbackMaterial->shader()->specificData<Common::ShaderData>()->Program;

        program->use();
    }
    else
    {
        auto shaderData = meshBehaviour->material()->shader()->specificData<Common::ShaderData>();

        if (shaderData == nullptr ||
            !shaderData->Program.is_valid())
        {
            if (!application()->renderer()->setup(meshBehaviour->material()->shader()))
            {
                // If can't setup shader
                shaderData = m_meshFallbackMaterial->shader()->specificData<Common::ShaderData>();
            }
            else
            {
                shaderData = meshBehaviour->material()->shader()->specificData<Common::ShaderData>();
            }
        }
        program = &shaderData->Program;
        program->use();

        applyShaderUniforms(meshBehaviour->material());
    }

    // Checking for VBO, VAO and EBO

    GLint location;

    if ((location = program->uniform_location("model")) != -1)
    {
        program->set_uniform(
            location,
            meshBehaviour->gameObject()->transform()->localToWorldMatrix()
        );
    }

    if ((location = program->uniform_location("view")) != -1)
    {
        program->set_uniform(
            location,
            application()->renderer()->activeCamera()->viewMatrix()
        );
    }

    if ((location = program->uniform_location("projection")) != -1)
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
                    castedLight->diffuse().toRGBVector() * 300.0f
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
