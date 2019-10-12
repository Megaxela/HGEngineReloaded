// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/Logging.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/Texture2DData.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DDataProcessor.hpp>

// HG::Utils
#include <HG/Utils/Surface.hpp>

namespace
{
GLuint getFilter(HG::Rendering::Base::Texture::Filtering filter)
{
    switch (filter)
    {
    case HG::Rendering::Base::Texture::Nearest:
        return GL_NEAREST;
    case HG::Rendering::Base::Texture::Linear:
        return GL_LINEAR;
    }

    return 0;
}

GLuint getWrapping(HG::Rendering::Base::Texture::Wrapping wrapping)
{
    switch (wrapping)
    {
    case HG::Rendering::Base::Texture::Repeat:
        return GL_REPEAT;
    case HG::Rendering::Base::Texture::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case HG::Rendering::Base::Texture::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case HG::Rendering::Base::Texture::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    }

    return 0;
}
} // namespace

namespace HG::Rendering::OpenGL::Common
{
bool Texture2DDataProcessor::setup(HG::Rendering::Base::RenderData* data, bool guarantee)
{
    auto texture = dynamic_cast<HG::Rendering::Base::Texture*>(data);

    if (texture == nullptr)
    {
        HGError() << "Got non texture render data in texture data processor. Types are corrupted.";
        exit(-1);
    }

    // Checking surface on texture
    Common::Texture2DData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = texture->castSpecificDataTo<Texture2DData>()) == nullptr)
    {
        externalData = new (application()->resourceCache()) Common::Texture2DData();
        texture->setSpecificData(externalData);
    }

    // Prepare storage if required
    if (texture->size(guarantee) != externalData->Size)
    {
        BENCH("Preparing storage");
        externalData->Texture = std::move(gl::texture_2d());

        externalData->Texture.set_min_filter(getFilter(texture->minificationMethod()));
        externalData->Texture.set_mag_filter(getFilter(texture->magnificationMethod()));

        // Setting size
        externalData->Size = texture->size();

        GLuint internalFormat = GL_RGBA8;

        switch (texture->internalFormat())
        {
        case Base::Texture::RGBA:
            internalFormat = GL_RGBA8;
            break;
        case Base::Texture::RGB:
            internalFormat = GL_RGB8;
            break;
        case Base::Texture::Depth:
            internalFormat = GL_DEPTH24_STENCIL8;
            break;
        }

        // Setting up storage
        externalData->Texture.set_storage(1,                    // Levels
                                          internalFormat,       // Internal format
                                          externalData->Size.x, // Width
                                          externalData->Size.y  // Height
        );

        // If storage was setup
        if (gl::error() == GL_NO_ERROR)
        {
            externalData->Allocated = true;
        }
    }

    // Load surface if surface is presented.
    // Surface is removed after texture is
    // filled.
    auto surface = texture->surface(guarantee);

    if (externalData->Allocated && surface && !externalData->Valid)
    {
        BENCH("Loading surface to texture");
        externalData->Valid = true;
        GLuint fileFormat   = GL_RGBA;

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
            HGError() << "Can't setup texture because of unknown texture format.";
            break;
        }

        // Loading data into texture
        externalData->Texture.set_sub_image(0,                // Level
                                            0,                // X offset
                                            0,                // Y Offset
                                            surface->Width,   // Width
                                            surface->Height,  // Height
                                            fileFormat,       // Format
                                            GL_UNSIGNED_BYTE, // Type
                                            surface->Data);
    }

    externalData->Texture.unbind();

    return externalData->Allocated && (externalData->Valid || (surface == nullptr && !externalData->Valid));
}

bool Texture2DDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto texture      = static_cast<HG::Rendering::Base::Texture*>(data);
    auto externalData = texture->castSpecificDataTo<Common::Texture2DData>();

    return externalData == nullptr || externalData->Texture.id() == gl::invalid_id ||
           externalData->Size != texture->size() || !externalData->Valid;
}

size_t Texture2DDataProcessor::getTarget()
{
    return HG::Rendering::Base::Texture::DataId;
}
} // namespace HG::Rendering::OpenGL::Common
