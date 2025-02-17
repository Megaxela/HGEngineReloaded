// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/CountStatistics.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/CubeMap.hpp>
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/CubeMap.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/Renderer.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/CubeMapTextureData.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Forward/CubeMapRenderer.hpp>
#include <HG/Rendering/OpenGL/Materials/SkyboxMaterial.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// gl
#include <gl/auxiliary/glm_uniforms.hpp>

namespace HG::Rendering::OpenGL::Forward
{
CubeMapRenderer::CubeMapRenderer() : m_vao(0), m_vbo(0)
{
}

void CubeMapRenderer::onDeinit()
{
    HGInfo("Deinitializing cubemap renderer");

    m_vao = std::move(gl::vertex_array(0));
    m_vbo = std::move(gl::buffer(0));

    delete m_skyboxMaterial;
    m_skyboxMaterial = nullptr;
}

void CubeMapRenderer::onInit()
{
    HGInfo("Initializing cubemap renderer");

    static float skyboxVertices[] = {
        // positions
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

        1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,

        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,

        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,

        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
        1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    };

    // Loading skybox material
    m_skyboxMaterial = application()->renderer()->materialCollection()->getMaterial<Materials::SkyboxMaterial>();

    // Setting up VAO/VBO
    m_vao = std::move(gl::vertex_array());
    m_vbo = std::move(gl::buffer());

    m_vao.bind();
    m_vbo.bind(GL_ARRAY_BUFFER);

    m_vbo.set_data(sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    m_vao.set_vertex_buffer(0, m_vbo, 0, sizeof(float) * 3);
    m_vao.set_attribute_enabled(0, true);
    m_vao.set_attribute_format(0, 3, GL_FLOAT);
}

void CubeMapRenderer::render(HG::Rendering::Base::RenderBehaviour* renderBehaviour)
{
    auto cubemapBehaviour = static_cast<HG::Rendering::Base::Behaviours::CubeMap*>(renderBehaviour);

    BENCH("Drawing cubemap");

    m_skyboxMaterial->set("skybox", cubemapBehaviour->cubeMap());
    m_skyboxMaterial->set("projection", application()->renderer()->activeCamera()->projectionMatrix());
    m_skyboxMaterial->set("view", glm::mat4(glm::mat3(application()->renderer()->activeCamera()->viewMatrix())));

    applyMaterialUniforms(application(), m_skyboxMaterial);
    useMaterial(application(), m_skyboxMaterial);

    m_vao.bind();

    gl::draw_arrays(GL_TRIANGLES, 0, 36);

    if (application()->countStatistics()->hasCounter(HG::Core::CountStatistics::CommonCounter::NumberOfVertices))
    {
        application()->countStatistics()->add(HG::Core::CountStatistics::CommonCounter::NumberOfVertices, 36);
    }

    application()->renderer()->setActiveCubeMap(cubemapBehaviour->cubeMap());

    gl::vertex_array::unbind();
    gl::cubemap_texture_array::unbind();
}

std::size_t CubeMapRenderer::getTarget()
{
    return HG::Rendering::Base::Behaviours::CubeMap::RenderBehaviourId;
}
} // namespace HG::Rendering::OpenGL::Forward
