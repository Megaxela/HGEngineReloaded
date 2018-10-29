// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Surface.hpp>

HG::Rendering::Base::Texture::Texture() :
    RenderData(DataId),
    m_surface(nullptr),
    m_minFiltering(Filtering::Nearest),
    m_magFiltering(Filtering::Nearest),
    m_sWrapping(Wrapping::Repeat),
    m_tWrapping(Wrapping::Repeat),
    m_size({0, 0}),
    m_internalFormat(Format::RGBA)
{

}

HG::Rendering::Base::Texture::Texture(HG::Utils::FutureHandler<HG::Utils::SurfacePtr> ptr,
                                      Filtering minification,
                                      Filtering magnification) :
    RenderData(DataId),
    m_surface(std::move(ptr)),
    m_minFiltering(minification),
    m_magFiltering(magnification),
    m_sWrapping(Wrapping::Repeat),
    m_tWrapping(Wrapping::Repeat),
    m_size({0, 0}),
    m_internalFormat(Format::RGBA)
{

}

HG::Rendering::Base::Texture::Texture(glm::ivec2 size,
                                      HG::Rendering::Base::Texture::Format format,
                                      HG::Rendering::Base::Texture::Filtering minification,
                                      HG::Rendering::Base::Texture::Filtering magnification) :
    RenderData(DataId),
    m_surface(nullptr),
    m_minFiltering(minification),
    m_magFiltering(magnification),
    m_sWrapping(Wrapping::Repeat),
    m_tWrapping(Wrapping::Repeat),
    m_size(size),
    m_internalFormat(format)
{

}

void HG::Rendering::Base::Texture::setSize(glm::ivec2 size)
{
    if (m_surface.get() != nullptr)
    {
        return;
    }

    m_size = size;
}

glm::ivec2 HG::Rendering::Base::Texture::size()
{
    auto surf = m_surface.get();
    if (surf)
    {
        m_size = {surf->Width, surf->Height};
    }

    return m_size;
}

HG::Utils::SurfacePtr HG::Rendering::Base::Texture::surface()
{
     return m_surface;
}

void HG::Rendering::Base::Texture::setSurface(HG::Utils::SurfaceFuturePtr ptr)
{
    m_surface = std::move(ptr);

    if (m_surface.get() == nullptr)
    {
        m_size = {0, 0};
    }
}

void HG::Rendering::Base::Texture::setMagnificationMethod(HG::Rendering::Base::Texture::Filtering value)
{
    m_magFiltering = value;
}

HG::Rendering::Base::Texture::Filtering HG::Rendering::Base::Texture::magnificationMethod() const
{
    return m_magFiltering;
}

void HG::Rendering::Base::Texture::setMinificationMethod(HG::Rendering::Base::Texture::Filtering value)
{
    m_minFiltering = value;
}

HG::Rendering::Base::Texture::Filtering HG::Rendering::Base::Texture::minificationMethod() const
{
    return m_minFiltering;
}

void HG::Rendering::Base::Texture::setSWrapping(HG::Rendering::Base::Texture::Wrapping method)
{
    m_sWrapping = method;
}

HG::Rendering::Base::Texture::Wrapping HG::Rendering::Base::Texture::sWrapping() const
{
    return m_sWrapping;
}

void HG::Rendering::Base::Texture::setTWrapping(HG::Rendering::Base::Texture::Wrapping method)
{
    m_tWrapping = method;
}

HG::Rendering::Base::Texture::Wrapping HG::Rendering::Base::Texture::tWrapping() const
{
    return m_tWrapping;
}

HG::Rendering::Base::Texture::Format HG::Rendering::Base::Texture::internalFormat() const
{
    return m_internalFormat;
}
