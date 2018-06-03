#pragma once

#include <type_traits>
#include <memory>
#include <CurrentLogger.hpp>
#include <Surface.hpp>

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes texture
     * for rendering (in GPU / prepared for GPU).
     */
    class Texture
    {
    public:

        /**
         * @brief Class, that describes abstract
         * external data for texture.
         */
        class TextureExternalData
        {
        public:

            /**
             * @brief Default virtual constructor.
             */
            virtual ~TextureExternalData() = default;
        };

        /**
         * @brief Constructor.
         */
        Texture();

        /**
         * @brief Constructor from surface.
         */
        explicit Texture(::UTILS_MODULE_NS::SurfacePtr ptr);

        /**
         * @brief Destructor.
         */
        ~Texture();

        /**
         * @brief Method for getting external data, casted
         * to some type.
         * @tparam T Type to cast.
         * @return Pointer to external data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<TextureExternalData, T>::value,
            T*
        >::type externalData() const
        {
            return static_cast<T*>(m_externalData);
        }

        /**
         * @brief Method for setting external data.
         * @tparam T Type of actual external data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<TextureExternalData, T>::value
        >::type setExternalData()
        {
#ifndef NDEBUG
            if (m_externalData)
            {
                Info() << "Recreating existing external data for texture.";
            }
#endif

            delete m_externalData;

            m_externalData = new T();
        };

        /**
         * @brief Method for clearing external data.
         */
        void clearExternalData();

        /**
         * @brief Method for getting binded surface to
         * texture.
         * @return Surface.
         */
        ::UTILS_MODULE_NS::SurfacePtr surface() const;

        /**
         * @brief Method for setting surface to texture.
         * @param ptr Pointer to surface.
         */
        void setSurface(::UTILS_MODULE_NS::SurfacePtr ptr);

    private:
        TextureExternalData* m_externalData;

        ::UTILS_MODULE_NS::SurfacePtr m_surface;
    };
}

