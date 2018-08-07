#include "MaterialCollection.hpp"
#include "Renderer.hpp"

RENDERING_BASE_MODULE_NS::MaterialCollection::MaterialCollection(CORE_MODULE_NS::ResourceManager* resourceManager,
                                                                 RENDERING_BASE_MODULE_NS::Renderer* pipeline) :
    m_resourceManager(resourceManager),
    m_renderer(pipeline)
{

}

RENDERING_BASE_MODULE_NS::MaterialCollection::~MaterialCollection()
{
    // Free all shaders
    for (auto&& [hash, shader] : m_shaders)
    {
        delete shader;
    }
}

void RENDERING_BASE_MODULE_NS::MaterialCollection::setup(RENDERING_BASE_MODULE_NS::Shader* shader)
{
    m_renderer->setup(shader);
}
