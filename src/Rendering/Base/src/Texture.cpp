#include <Texture.hpp>
#include <Surface.hpp>

HG::Rendering::Base::Texture::Texture() :
    RenderData(DataId),
    m_surface(nullptr),
    m_minFiltering(Filtering::Nearest),
    m_magFiltering(Filtering::Nearest),
    m_sWrapping(Wrapping::Repeat),
    m_tWrapping(Wrapping::Repeat)
{

}

HG::Rendering::Base::Texture::Texture(HG::Utils::FutureHandler<HG::Utils::SurfacePtr>::Ptr ptr,
                                      Filtering minification,
                                      Filtering magnification) :
    RenderData(DataId),
    m_surface(std::move(ptr)),
    m_minFiltering(minification),
    m_magFiltering(magnification),
    m_sWrapping(Wrapping::Repeat),
    m_tWrapping(Wrapping::Repeat)
{

}

glm::ivec2 HG::Rendering::Base::Texture::size() const
{
    return m_size;
}

HG::Utils::SurfacePtr HG::Rendering::Base::Texture::surface()
{
     return *m_surface;
}

void HG::Rendering::Base::Texture::setSurface(HG::Utils::FutureHandler<HG::Utils::SurfacePtr>::Ptr ptr)
{
    m_surface = std::move(ptr);

//    if (m_surface.get())
//    {
//        m_size = glm::vec2(m_surface->Width, m_surface->Height);
//    }
//    else
//    {
//        m_size = glm::ivec2();
//    }
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
