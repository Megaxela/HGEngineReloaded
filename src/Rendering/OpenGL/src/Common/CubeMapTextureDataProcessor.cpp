// HG::Rendering::OpenGL
#include <Common/CubeMapTextureDataProcessor.hpp>
#include <Common/CubeMapTextureData.hpp>

// HG::Rendering::Base
#include <CubeMapTexture.hpp>

// HG::Utils
#include <Surface.hpp>

// gl
#include <gl/all.hpp>

namespace
{
    void setupCubeMapSide(const HG::Utils::SurfacePtr& surface,
                          gl::cubemap_texture& texture,
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
        glTexImage2D(
            side,
            0,
            GL_RGBA8,
            surface->Width,
            surface->Height,
            0,
            fileFormat,
            GL_UNSIGNED_BYTE,
            surface->Data
        );

//    texture.set_storage(
//        1,       // Levels
//        GL_RGBA8, // Internal format
//        surface->Width,
//        surface->Height
//    );
//    texture.set_sub_image(
//        0, // Level
//        0, // X offset
//        0, // Y Offset
//        surface->Width,  // Width
//        surface->Height, // Height
//        fileFormat,       // Format
//        GL_UNSIGNED_BYTE, // Type
//        surface->Data
//    );
    }
}

bool HG::Rendering::OpenGL::Common::CubeMapTextureDataProcessor::setup(HG::Rendering::Base::RenderData* data)
{
    auto texture = static_cast<HG::Rendering::Base::CubeMapTexture*>(data);

    // If one of surfaces are not available
    // post error
    if (texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Right ) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Left  ) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Top   ) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Bottom) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Front ) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Back  ) == nullptr)
    {
        Error() << "Can't setup not fully set up cube map texture.";
        return false;
    }

    Common::CubeMapTextureData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = static_cast<CubeMapTextureData*>(texture->specificData())) == nullptr)
    {
        externalData = new Common::CubeMapTextureData();
        texture->setSpecificData(externalData);
    }

    // Setting up textures
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Right),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Left),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Top),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Bottom),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Front),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Back),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    );

    externalData->Texture.set_min_filter(GL_LINEAR);
    externalData->Texture.set_mag_filter(GL_LINEAR);

    externalData->Texture.set_wrap_s(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_t(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_r(GL_CLAMP_TO_EDGE);

    return true;
}

size_t HG::Rendering::OpenGL::Common::CubeMapTextureDataProcessor::getTarget()
{
    return HG::Rendering::Base::CubeMapTexture::Id;
}
