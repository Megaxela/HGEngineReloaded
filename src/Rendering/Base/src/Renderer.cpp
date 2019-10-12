// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/CountStatistics.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/Gizmos.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

namespace HG::Rendering::Base
{
Renderer::Renderer(HG::Core::Application* application) :
    m_parentApplication(application),
    m_pipeline(nullptr),
    m_gizmos(new Gizmos()),
    m_materialCollection(new MaterialCollection(application->resourceManager(), this)),
    m_activeCamera(nullptr),
    m_defaultRenderTarget(new (application->resourceCache()) RenderTarget({0, 0}, true)),
    m_activeCubemap(nullptr)
{
    HGDebug() << "Creating renderer.";

    m_parentApplication->countStatistics()->addCounter(HG::Core::CountStatistics::CommonCounter::NumberOfVertices,
                                                       HG::Core::CountStatistics::CounterType::LastFrame);
}

Renderer::~Renderer()
{
    HGDebug() << "Destroying renderer.";
    m_parentApplication->countStatistics()->removeCounter(HG::Core::CountStatistics::CommonCounter::NumberOfVertices);

    delete m_pipeline;
    delete m_gizmos;
    delete m_materialCollection;
    delete m_defaultRenderTarget;
}

RenderTarget* Renderer::defaultRenderTarget() const
{
    return m_defaultRenderTarget;
}

void Renderer::setPipeline(RenderingPipeline* pipeline)
{
    m_pipeline = pipeline;
}

bool Renderer::init()
{
    if (m_pipeline == nullptr)
    {
        HGError() << "No rendering pipeline set.";
        return false;
    }

    return m_pipeline->init();
}

void Renderer::deinit()
{
    if (m_pipeline == nullptr)
    {
        HGError() << "No rendering pipeline set.";
        return;
    }

    return m_pipeline->deinit();
}

void Renderer::render(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& gameObjects)
{
    if (m_pipeline == nullptr)
    {
        HGError() << "No rendering pipeline set.";
        return;
    }

    m_pipeline->render(gameObjects.current());

    m_gizmos->clear();
}

Gizmos* Renderer::gizmos() const
{
    return m_gizmos;
}

RenderingPipeline* Renderer::pipeline() const
{
    return m_pipeline;
}

MaterialCollection* Renderer::materialCollection() const
{
    return m_materialCollection;
}

Camera* Renderer::activeCamera() const
{
    return m_activeCamera;
}

void Renderer::setActiveCamera(Camera* camera)
{
    if (camera->gameObject() == nullptr)
    {
        return;
    }

    m_activeCamera = camera;
}

CubeMap* Renderer::activeCubeMap() const
{
    return m_activeCubemap;
}

void Renderer::setActiveCubeMap(CubeMap* cubemap)
{
    m_activeCubemap = cubemap;
}

bool Renderer::setup(RenderData* data, bool guarantee)
{
    if (m_pipeline == nullptr)
    {
        HGInfo() << "There is no rendering pipeline to setup.";
        return false;
    }

    return m_pipeline->setup(data, guarantee);
}

bool Renderer::needSetup(RenderData* data) const
{
    if (m_pipeline == nullptr)
    {
        HGInfo() << "There is no rendering pipeline to check for setup requirement.";
        return false;
    }

    return m_pipeline->needSetup(data);
}

HG::Utils::Color Renderer::getTexturePixel(Texture* texture, glm::ivec2 pos)
{
    if (m_pipeline == nullptr)
    {
        throw std::runtime_error("No pipeline to work with.");
    }

    std::uint8_t buffer[4];
    m_pipeline->getTextureRegion(texture, pos, pos + glm::ivec2(1, 1), buffer);

    return HG::Utils::Color::fromRGB(buffer[0], buffer[1], buffer[2], buffer[3]);
}
} // namespace HG::Rendering::Base
