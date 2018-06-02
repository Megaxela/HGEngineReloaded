#include "Texture.hpp"

RENDERING_BASE_MODULE_NS::Texture::Texture() :
    m_externalData(nullptr),
    m_surface(nullptr)
{

}

RENDERING_BASE_MODULE_NS::Texture::~Texture()
{
    clearExternalData();
}

void RENDERING_BASE_MODULE_NS::Texture::clearExternalData()
{
    delete m_externalData;
    m_externalData = nullptr;
}

::UTILS_MODULE_NS::SurfacePtr RENDERING_BASE_MODULE_NS::Texture::surface() const
{
    return m_surface;
}

void RENDERING_BASE_MODULE_NS::Texture::setSurface(::UTILS_MODULE_NS::SurfacePtr ptr)
{
    m_surface = std::move(ptr);
}
