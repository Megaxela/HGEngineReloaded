// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/Logging.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/CubeMapTextureData.hpp>
#include <HG/Rendering/OpenGL/Common/CubeMapTextureDataProcessor.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/CubeMap.hpp>

// HG::Utils
#include <HG/Utils/Surface.hpp>

// gl
#include <gl/all.hpp>

namespace
{
void setupCubeMapSide(const HG::Utils::SurfacePtr& surface,
                      gl::cubemap_texture_array& texture,
                      GLuint side,
                      bool guarantee)
{
    // todo: Fix this `unused` variable.
    (void)texture;
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
        HGErrorF() << "Can't setup texture because of unknown texture format.";
        break;
    }

    // Loading data into texture
    texture.set_sub_image(0,                // Level
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
} // namespace

namespace HG::Rendering::OpenGL::Common
{
bool CubeMapTextureDataProcessor::setup(HG::Rendering::Base::RenderData* data, bool guarantee)
{
    auto texture = static_cast<HG::Rendering::Base::CubeMap*>(data);

    // If one of surfaces are not available - dont do anything
    // todo: It would be fun if enable side-side loading.

    CubeMapTextureData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = texture->castSpecificDataTo<CubeMapTextureData>()) == nullptr)
    {
        externalData = new (application()->resourceCache()) CubeMapTextureData();
        texture->setSpecificData(externalData);
    }

    auto surf = texture->getSideSurface(HG::Rendering::Base::CubeMap::Side::Right);

    if (surf && !externalData->StoragePrepared)
    {
        BENCH("Creating texture storage");
        externalData->StoragePrepared = true;

        externalData->Texture.set_storage(1,        // Levels
                                          GL_RGBA8, // Internal format
                                          surf->Width,
                                          surf->Height,
                                          6);
    }

    if (externalData->StoragePrepared)
    {
        for (int side = HG::Rendering::Base::CubeMap::Side::Right; side < HG::Rendering::Base::CubeMap::Side::Last;
             ++side)
        {
            BENCH("Loading side data to texture");
            if ((externalData->LoadedSides & (1 << side)) == 0)
            {
                auto surface = texture->getSideSurface(static_cast<Base::CubeMap::Side>(side), guarantee);

                // Surface was not ready
                if (!surface)
                {
                    continue;
                }

                externalData->LoadedSides |= (1 << side);
                setupCubeMapSide(surface, externalData->Texture, static_cast<GLuint>(side), guarantee);
            }
        }
    }

    externalData->Texture.set_min_filter(GL_LINEAR);
    externalData->Texture.set_mag_filter(GL_LINEAR);

    externalData->Texture.set_wrap_s(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_t(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_r(GL_CLAMP_TO_EDGE);

    if (externalData->LoadedSides == ((1 << 6) - 1))
    {
        externalData->Valid = true;
    }

    return true;
}

bool CubeMapTextureDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto externalData = data->castSpecificDataTo<CubeMapTextureData>();

    return externalData == nullptr || externalData->LoadedSides != ((1 << 6) - 1) || // Fully loaded bitfield
           !externalData->Valid || externalData->Texture.id() == gl::invalid_id;
}

size_t CubeMapTextureDataProcessor::getTarget()
{
    return HG::Rendering::Base::CubeMap::Id;
}
} // namespace HG::Rendering::OpenGL::Common
