// HG::Rendering::OpenGL
#include <Common/CubeMapTextureDataProcessor.hpp>
#include <Common/CubeMapTextureData.hpp>

// HG::Rendering::Base
#include <CubeMap.hpp>

// HG::Utils
#include <Surface.hpp>

// gl
#include <gl/all.hpp>

namespace
{
    void setupCubeMapSide(const HG::Utils::SurfacePtr& surface,
                          gl::cubemap_texture_array& texture,
                          GLuint side)
    {
        // todo: Fix this `unused` variable.
        (void) texture;
        GLuint fileFormat = GL_RGB;

        // Getting type
        switch (surface->Bpp)
        {
        case 1:
            fileFormat = GL_RED;
            break;

        case 2:
            fileFormat = GL_RG;
            break;

        case 3:
            fileFormat = GL_RGB;
            break;

        case 4:
            fileFormat = GL_RGBA;
            break;

        default:
            ErrorF() << "Can't setup texture because of unknown texture format.";
            break;
        }

        // Loading data into texture
        texture.set_sub_image(
            0,                // Level
            0,                // X offset
            0,                // Y Offset
            side,             // Z Offset
            surface->Width,   // Width
            surface->Height,  // Height
            1,                // Depth
            fileFormat,       // Format
            GL_UNSIGNED_BYTE, // Type
            surface->Data     // Actual data
        );
    }
}

bool HG::Rendering::OpenGL::Common::CubeMapTextureDataProcessor::setup(HG::Rendering::Base::RenderData* data)
{
    auto texture = static_cast<HG::Rendering::Base::CubeMap*>(data);

    // If one of surfaces are not available - dont do anything
    // todo: It would be fun if enable side-side loading.

    Common::CubeMapTextureData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = static_cast<CubeMapTextureData*>(texture->specificData())) == nullptr)
    {
        externalData = new Common::CubeMapTextureData();
        texture->setSpecificData(externalData);
    }

    auto surf = texture->getSideSurface(HG::Rendering::Base::CubeMap::Side::Right);

    if (surf && !externalData->StoragePrepared)
    {
        externalData->StoragePrepared = true;

        externalData->Texture.set_storage(
            1,       // Levels
            GL_RGBA8, // Internal format
            surf->Width,
            surf->Height,
            6
        );
    }

    // Setting up textures
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A

    for (int side = HG::Rendering::Base::CubeMap::Side::Right;
         side < HG::Rendering::Base::CubeMap::Side::Last;
         ++side)
    {
        if ((externalData->LoadedSides & (1 << side)) == 0)
        {
            auto surface = texture->getSideSurface(static_cast<Base::CubeMap::Side>(side));

            // Surface was not ready
            if (!surface)
            {
                continue;
            }

            externalData->LoadedSides |= (1 << side);
            setupCubeMapSide(surface, externalData->Texture, static_cast<GLuint>(side));
        }
    }

    externalData->Texture.set_min_filter(GL_LINEAR);
    externalData->Texture.set_mag_filter(GL_LINEAR);

    externalData->Texture.set_wrap_s(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_t(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_r(GL_CLAMP_TO_EDGE);

    return true;
}

bool HG::Rendering::OpenGL::Common::CubeMapTextureDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto externalData = static_cast<CubeMapTextureData*>(data->specificData());

    return externalData == nullptr ||
           externalData->LoadedSides != ((1 << 6) - 1) || // Fully loaded bitfield
           !externalData->Texture.is_valid();
}

size_t HG::Rendering::OpenGL::Common::CubeMapTextureDataProcessor::getTarget()
{
    return HG::Rendering::Base::CubeMap::Id;
}
