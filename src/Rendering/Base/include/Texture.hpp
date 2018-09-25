#pragma once

// HG::Rendering::Base
#include <RenderData.hpp>

// HG::Utils
#include <FutureHandler.hpp>

// C++ STL
#include <memory>

// GLM
#include <glm/vec2.hpp>

namespace HG::Utils
{
    class Surface;
    using SurfacePtr = std::shared_ptr<Surface>;
    using SurfaceFuturePtr = FutureHandler<SurfacePtr>;
}

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes texture
     * for rendering (in GPU / prepared for GPU).
     */
    class Texture : public HG::Rendering::Base::RenderData
    {
    public:

        static constexpr std::size_t DataId = 3;

        /**
         * @brief Texture filtering methods.
         */
        enum Filtering
        {
            Nearest,
            Linear
        };

        /**
         * @brief Texture wrapping methods.
         */
        enum Wrapping
        {
            Repeat,
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder
        };

        /**
         * @brief Texture internal format.
         */
        enum Format
        {
            RGBA,
            Depth
        };

        /**
         * @brief Constructor.
         */
        Texture();

        /**
         * @brief Constructor from surface.
         * It will set format to RGBA.
         */
        explicit Texture(HG::Utils::FutureHandler<HG::Utils::SurfacePtr> ptr,
                         Filtering minification =Nearest,
                         Filtering magnification=Nearest);

        /**
         * @brief Initializer constructor.
         * @param size Texture size.
         * @param format Texture format.
         * @param minification Minification filter.
         * @param magnification Magnification filter.
         */
        Texture(glm::ivec2 size,
                Format format,
                Filtering minification =Nearest,
                Filtering magnification=Nearest);

        /**
         * @brief Method for getting binded surface to
         * texture.
         * @return Surface.
         */
        HG::Utils::SurfacePtr surface();

        /**
         * @brief Method for setting surface to texture.
         * @param ptr Pointer to surface.
         */
        void setSurface(HG::Utils::SurfaceFuturePtr ptr);

        /**
         * @brief Method for getting texture size in pixels.
         * @return Texture size in pixels.
         */
        glm::ivec2 size();

        /**
         * @brief Method for changing current magnification
         * method. This value change requires setup in renderer.
         * @param value Method value.
         */
        void setMagnificationMethod(Filtering value);

        /**
         * @brief Method for getting current magnification
         * method.
         * @return Method value.
         */
        Filtering magnificationMethod() const;

        /**
         * @brief Method for changing current minification
         * method. This value change requires setup in renderer.
         * @param value Method value.
         */
        void setMinificationMethod(Filtering value);

        /**
         * @brief Method for getting current minification
         * method.
         * @return Method value.
         */
        Filtering minificationMethod() const;

        /**
         * @brief Method for setting s coordinate wrapping
         * method.
         * @param method Method value.
         */
        void setSWrapping(Wrapping method);

        /**
         * @brief Method for getting s coordinate wrapping
         * method.
         * @return Method value.
         */
        Wrapping sWrapping() const;

        /**
         * @brief Method for setting t coordinate wrapping
         * method.
         * @param method Method value.
         */
        void setTWrapping(Wrapping method);

        /**
         * @brief Method for getting t coordinate wrapping
         * method.
         * @return Method value.
         */
        Wrapping tWrapping() const;

        /**
         * @brief Method for getting internal texture
         * format.
         */
        Format internalFormat() const;

    private:
        HG::Utils::SurfaceFuturePtr  m_surface;

        Filtering m_minFiltering;
        Filtering m_magFiltering;
        Wrapping m_sWrapping;
        Wrapping m_tWrapping;
        Format m_internalFormat;

        glm::ivec2 m_size;
    };
}

