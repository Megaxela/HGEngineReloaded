// HG::Rendering::Base
#include <MaterialCollection.hpp>
#include <Renderer.hpp>

HG::Rendering::Base::MaterialCollection::MaterialCollection(HG::Core::ResourceManager* resourceManager,
                                                            HG::Rendering::Base::Renderer* pipeline) :
    m_resourceManager(resourceManager),
    m_renderer(pipeline)
{

}

HG::Rendering::Base::MaterialCollection::~MaterialCollection()
{
    clearCache();
}

void HG::Rendering::Base::MaterialCollection::setup(HG::Rendering::Base::Shader* shader)
{
    m_renderer->setup(shader);
}

void HG::Rendering::Base::MaterialCollection::clearCache()
{
    // Free all shaders
    for (auto&& [hash, shader] : m_shaders)
    {
        delete shader;
    }

    m_shaders.clear();
}
