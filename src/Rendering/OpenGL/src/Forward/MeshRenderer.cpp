// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/CountStatistics.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/MeshData.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Forward/MeshRenderer.hpp>
#include <HG/Rendering/OpenGL/Materials/MeshFallbackMaterial.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/Mesh.hpp>
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/Lights/AbstractLight.hpp>
#include <HG/Rendering/Base/Lights/DirectionalLight.hpp>
#include <HG/Rendering/Base/Lights/PointLight.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/RenderOverride.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>
#include <HG/Utils/Mesh.hpp>

// GLM
#include <gl/auxiliary/glm_uniforms.hpp>

// gl
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Forward
{
MeshRenderer::MeshRenderer() : m_meshFallbackMaterial(nullptr)
{
    constexpr int count = 128;
    m_pointLightNames.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        m_pointLightNames.emplace_back("pointLights[" + std::to_string(i) + "].position",
                                       "pointLights[" + std::to_string(i) + "].constant",
                                       "pointLights[" + std::to_string(i) + "].linear",
                                       "pointLights[" + std::to_string(i) + "].quadratic",
                                       "pointLights[" + std::to_string(i) + "].diffuse");
    }
}

void MeshRenderer::onInit()
{
    HGInfo("Initializing mesh renderer");

    m_meshFallbackMaterial =
        application()->renderer()->materialCollection()->getMaterial<Materials::MeshFallbackMaterial>();
}

void MeshRenderer::onDeinit()
{
    HGInfo("Deinitializing sprite renderer");

    delete m_meshFallbackMaterial;
    m_meshFallbackMaterial = nullptr;
}

void MeshRenderer::render(HG::Rendering::Base::RenderBehaviour* renderBehaviour)
{
    auto meshBehaviour = static_cast<HG::Rendering::Base::Behaviours::Mesh*>(renderBehaviour);
    auto data          = meshBehaviour->castSpecificDataTo<Common::MeshData>();
    auto override      = application()->renderer()->pipeline()->renderOverride();

    // todo: On errors, render "error" mesh instead.
    if (application()->renderer()->needSetup(meshBehaviour))
    {
        if (!application()->renderer()->setup(meshBehaviour))
        {
            return;
        }

        data = meshBehaviour->castSpecificDataTo<Common::MeshData>();
    }

    BENCH("Drawing mesh");

    HG::Rendering::Base::Material* activeMaterial = nullptr;

    if (override == nullptr || override->material == nullptr)
    {
        if (meshBehaviour->material() == nullptr || meshBehaviour->material()->shader() == nullptr)
        {
            activeMaterial = m_meshFallbackMaterial;
        }
        else
        {
            activeMaterial = meshBehaviour->material();
        }
    }
    else
    {
        activeMaterial = override->material;
    }

    if (application()->renderer()->activeCubeMap())
    {
        activeMaterial->set("cubemap", application()->renderer()->activeCubeMap());
    }

    // No active camera. No rendering.
    if (application()->renderer()->activeCamera() == nullptr)
    {
        return;
    }

    // Checking for VBO, VAO and EBO

    activeMaterial->set("camera",
                        application()->renderer()->activeCamera()->gameObject()->transform()->globalPosition());

    activeMaterial->set("model", meshBehaviour->gameObject()->transform()->localToWorldMatrix());

    activeMaterial->set("view", application()->renderer()->activeCamera()->viewMatrix());

    activeMaterial->set("projection", application()->renderer()->activeCamera()->projectionMatrix());

    // Setting lighting uniforms
    auto& lights = HG::Rendering::Base::AbstractLight::totalLights();

    int pointLightIndex       = 0;
    int directionalLightIndex = 0;
    int spotLightIndex        = 0;

    for (auto&& light : lights)
    {
        if (light->gameObject() == nullptr || !light->gameObject()->isEnabled() || !light->isEnabled())
        {
            continue;
        }

        switch (light->lightType())
        {
        case HG::Rendering::Base::AbstractLight::Type::Point: {
            auto castedLight = static_cast<HG::Rendering::Base::Lights::PointLight*>(light);

            activeMaterial->set(m_pointLightNames[pointLightIndex].position,
                                castedLight->gameObject()->transform()->globalPosition());

            activeMaterial->set(m_pointLightNames[pointLightIndex].constant, castedLight->constant());

            activeMaterial->set(m_pointLightNames[pointLightIndex].linear, castedLight->linear());

            activeMaterial->set(m_pointLightNames[pointLightIndex].quadratic, castedLight->quadratic());

            activeMaterial->set(m_pointLightNames[pointLightIndex].diffuse,
                                castedLight->color().toRGBVector() * 300.0f);

            ++pointLightIndex;
            break;
        }
        case HG::Rendering::Base::AbstractLight::Type::Directional: {
            //            auto castedLight = static_cast<HG::Rendering::Base::Lights::DirectionalLight*>(light);

            // todo: Finish directional light uniform info
            ++directionalLightIndex;
            break;
        }
        case HG::Rendering::Base::AbstractLight::Type::Spot: {
            //            auto castedLight = static_cast<HG::Rendering::Base::Lights::*>(light);

            // todo: Finish stop light uniform info
            ++spotLightIndex;
            break;
        }
        }
    }

    activeMaterial->set("numberOfPointLights", pointLightIndex);
    activeMaterial->set("numberOfDirectionalLights", directionalLightIndex);
    activeMaterial->set("numberOfSpotLights", spotLightIndex);

    activeMaterial->set("viewPos",
                        application()->renderer()->activeCamera()->gameObject()->transform()->globalPosition());

    applyMaterialUniforms(application(), activeMaterial);
    useMaterial(application(), activeMaterial);

    data->VAO.bind();

    gl::draw_range_elements(GL_TRIANGLES, // mode
                            0,            // start
                            static_cast<GLuint>(data->Count),
                            static_cast<GLsizei>(data->Count),
                            GL_UNSIGNED_INT);

    if (application()->countStatistics()->hasCounter(HG::Core::CountStatistics::CommonCounter::NumberOfVertices))
    {
        application()->countStatistics()->add(HG::Core::CountStatistics::CommonCounter::NumberOfVertices, data->Count);
    }

    data->VAO.unbind();
}

std::size_t MeshRenderer::getTarget()
{
    return HG::Rendering::Base::Behaviours::Mesh::RenderBehaviourId;
}
} // namespace HG::Rendering::OpenGL::Forward
