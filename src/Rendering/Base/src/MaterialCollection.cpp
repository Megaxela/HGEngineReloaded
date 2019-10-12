// HG::Rendering::Base
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/Renderer.hpp>

namespace HG::Rendering::Base
{
MaterialCollection::MaterialCollection(HG::Core::ResourceManager* resourceManager, Renderer* pipeline) :
    m_resourceManager(resourceManager),
    m_renderer(pipeline)
{
}

MaterialCollection::~MaterialCollection()
{
    clearCache();
}

void MaterialCollection::setup(Shader* shader)
{
    m_renderer->setup(shader, true);
}

void MaterialCollection::clearCache()
{
    // Free all shaders
    for (auto&& [hash, shader] : m_shaders)
    {
        delete shader;
    }

    m_shaders.clear();
}
} // namespace HG::Rendering::Base
