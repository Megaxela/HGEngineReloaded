#include "MaterialCollection.hpp"
#include "Renderer.hpp"

HG::Rendering::Base::MaterialCollection::MaterialCollection(HG::Core::ResourceManager* resourceManager,
                                                                 HG::Rendering::Base::Renderer* pipeline) :
    m_resourceManager(resourceManager),
    m_renderer(pipeline)
{

}

HG::Rendering::Base::MaterialCollection::~MaterialCollection()
{
    // Free all shaders
    for (auto&& [hash, shader] : m_shaders)
    {
        delete shader;
    }
}

void HG::Rendering::Base::MaterialCollection::setup(HG::Rendering::Base::Shader* shader)
{
    m_renderer->setup(shader);
}
