#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DData.hpp>
#include <CurrentLogger.hpp>
#include <HG/Utils/Surface.hpp>

namespace
{
    GLuint getFilter(HG::Rendering::Base::Texture::Filtering filter)
    {
        switch (filter)
        {
        case HG::Rendering::Base::Texture::Nearest: return GL_NEAREST;
        case HG::Rendering::Base::Texture::Linear:  return GL_LINEAR;
        }

        return 0;
    }

    GLuint getWrapping(HG::Rendering::Base::Texture::Wrapping wrapping)
    {
        switch (wrapping)
        {
        case HG::Rendering::Base::Texture::Repeat:         return GL_REPEAT;
        case HG::Rendering::Base::Texture::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case HG::Rendering::Base::Texture::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case HG::Rendering::Base::Texture::ClampToBorder:  return GL_CLAMP_TO_BORDER;
        }

        return 0;
    }
}

bool HG::Rendering::OpenGL::Common::Texture2DDataProcessor::setup(HG::Rendering::Base::RenderData* data)
{
    auto texture = dynamic_cast<HG::Rendering::Base::Texture*>(data);

    if (texture == nullptr)
    {
        Error() << "Got non texture render data in texture data processor. Types are corrupted.";
        exit(-1);
    }

    // Checking surface on texture
    Common::Texture2DData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = static_cast<Texture2DData*>(texture->specificData())) == nullptr)
    {
        externalData = new Common::Texture2DData();
        texture->setSpecificData(externalData);
    }

    externalData->Texture.set_min_filter(
        getFilter(texture->minificationMethod())
    );
    externalData->Texture.set_mag_filter(
        getFilter(texture->magnificationMethod())
    );

    // Prepare storage if required
    if (texture->size() != externalData->Size)
    {
        // Setting size
        externalData->Size = texture->size();

        GLuint internalFormat = GL_RGBA8;

        switch (texture->internalFormat())
        {
        case Base::Texture::RGBA:
            internalFormat = GL_RGBA8;
            break;
        case Base::Texture::Depth:
            internalFormat = GL_DEPTH24_STENCIL8;
            break;
        }

        // Setting up storage
        externalData->Texture.set_storage(
            1, // Levels
            internalFormat, // Internal format
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
    if (externalData->Allocated &&
        texture->surface() &&
        !externalData->Valid)
    {
        externalData->Valid = true;
        GLuint fileFormat = GL_RGBA;

        // Getting type
        switch (texture->surface()->Bpp)
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
            Error() << "Can't setup texture because of unknown texture format.";
            break;
        }

        // Loading data into texture
        externalData->Texture.set_sub_image(
            0, // Level
            0, // X offset
            0, // Y Offset
            texture->surface()->Width,  // Width
            texture->surface()->Height, // Height
            fileFormat,       // Format
            GL_UNSIGNED_BYTE, // Type
            texture->surface()->Data
        );
    }

    externalData->Texture.unbind();

    return true;
}

bool HG::Rendering::OpenGL::Common::Texture2DDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto texture = static_cast<HG::Rendering::Base::Texture*>(data);
    auto externalData = static_cast<Common::Texture2DData*>(texture->specificData());

    return  externalData == nullptr ||
           !externalData->Texture.is_valid() ||
            externalData->Size != texture->size() ||
           !externalData->Valid;
}

size_t HG::Rendering::OpenGL::Common::Texture2DDataProcessor::getTarget()
{
    return HG::Rendering::Base::Texture::DataId;
}
