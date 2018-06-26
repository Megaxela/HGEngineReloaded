#include "Texture.hpp"

RENDERING_BASE_MODULE_NS::Texture::Texture() :
    m_externalData(nullptr),
    m_surface(nullptr),
    m_minFiltering(Filtering::Nearest),
    m_magFiltering(Filtering::Nearest),
    m_sWrapping(Wrapping::Repeat),
    m_tWrapping(Wrapping::Repeat)
{

}

RENDERING_BASE_MODULE_NS::Texture::Texture(::UTILS_MODULE_NS::SurfacePtr ptr) :
    m_externalData(nullptr),
    m_surface(std::move(ptr)),
    m_minFiltering(Filtering::Nearest),
    m_magFiltering(Filtering::Nearest),
    m_sWrapping(Wrapping::Repeat),
    m_tWrapping(Wrapping::Repeat)
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

void RENDERING_BASE_MODULE_NS::Texture::setMagnificationMethod(RENDERING_BASE_MODULE_NS::Texture::Filtering value)
{
    m_magFiltering = value;
}

RENDERING_BASE_MODULE_NS::Texture::Filtering RENDERING_BASE_MODULE_NS::Texture::magnificationMethod() const
{
    return m_magFiltering;
}

void RENDERING_BASE_MODULE_NS::Texture::setMinificationMethod(RENDERING_BASE_MODULE_NS::Texture::Filtering value)
{
    m_minFiltering = value;
}

RENDERING_BASE_MODULE_NS::Texture::Filtering RENDERING_BASE_MODULE_NS::Texture::minificationMethod() const
{
    return m_minFiltering;
}

void RENDERING_BASE_MODULE_NS::Texture::setSWrapping(RENDERING_BASE_MODULE_NS::Texture::Wrapping method)
{
    m_sWrapping = method;
}

RENDERING_BASE_MODULE_NS::Texture::Wrapping RENDERING_BASE_MODULE_NS::Texture::sWrapping() const
{
    return m_sWrapping;
}

void RENDERING_BASE_MODULE_NS::Texture::setTWrapping(RENDERING_BASE_MODULE_NS::Texture::Wrapping method)
{
    m_tWrapping = method;
}

RENDERING_BASE_MODULE_NS::Texture::Wrapping RENDERING_BASE_MODULE_NS::Texture::tWrapping() const
{
    return m_tWrapping;
}
