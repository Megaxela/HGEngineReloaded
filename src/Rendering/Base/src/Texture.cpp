// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Surface.hpp>

namespace HG::Rendering::Base
{
Texture::Texture() :
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

Texture::Texture(HG::Utils::FutureHandler<HG::Utils::SurfacePtr> ptr, Filtering minification, Filtering magnification) :
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

Texture::Texture(glm::ivec2 size,
                 Texture::Format format,
                 Texture::Filtering minification,
                 Texture::Filtering magnification) :
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

void Texture::setSize(glm::ivec2 size)
{
    if (m_surface.get() != nullptr)
    {
        return;
    }

    m_size = size;
}

glm::ivec2 Texture::size(bool guarantee)
{
    auto surf = guarantee ? m_surface.guaranteeGet() : m_surface.get();
    if (surf)
    {
        m_size = {surf->Width, surf->Height};
    }

    return m_size;
}

HG::Utils::SurfacePtr Texture::surface(bool guarantee)
{
    return guarantee ? m_surface.guaranteeGet() : m_surface.get();
}

void Texture::setSurface(HG::Utils::SurfaceFuturePtr ptr)
{
    m_surface = std::move(ptr);

    if (m_surface.get() == nullptr)
    {
        m_size = {0, 0};
    }
}

void Texture::setMagnificationMethod(Texture::Filtering value)
{
    m_magFiltering = value;
}

Texture::Filtering Texture::magnificationMethod() const
{
    return m_magFiltering;
}

void Texture::setMinificationMethod(Texture::Filtering value)
{
    m_minFiltering = value;
}

Texture::Filtering Texture::minificationMethod() const
{
    return m_minFiltering;
}

void Texture::setSWrapping(Texture::Wrapping method)
{
    m_sWrapping = method;
}

Texture::Wrapping Texture::sWrapping() const
{
    return m_sWrapping;
}

void Texture::setTWrapping(Texture::Wrapping method)
{
    m_tWrapping = method;
}

Texture::Wrapping Texture::tWrapping() const
{
    return m_tWrapping;
}

Texture::Format Texture::internalFormat() const
{
    return m_internalFormat;
}
} // namespace HG::Rendering::Base
