#include <Texture.hpp>
#include "Common/Texture2DDataProcessor.hpp"
#include <Common/Texture2DData.hpp>
#include <CurrentLogger.hpp>
#include <Surface.hpp>

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

    // Checking surface on texture
    if (texture->surface() == nullptr)
    {
//        Error() << "Can't setup texture without surface on it.";
        return false;
    }

    Common::Texture2DData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = texture->specificData<Common::Texture2DData>()) == nullptr)
    {
        externalData = texture->setSpecificData<Common::Texture2DData>();
    }

    if (!externalData->Texture.is_valid())
    {
        externalData->Texture = std::move(gl::texture_2d());
    }

    externalData->Texture.set_min_filter(
        getFilter(
            texture->minificationMethod()
        )
    );
    externalData->Texture.set_mag_filter(
        getFilter(
            texture->magnificationMethod()
        )
    );

    GLuint fileFormat;

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

    // Setting up storage
    externalData->Texture.set_storage(
        1,       // Levels
        GL_RGBA8, // Internal format
        texture->surface()->Width,
        texture->surface()->Height
    );

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

    externalData->Texture.unbind();

    return true;
}

size_t HG::Rendering::OpenGL::Common::Texture2DDataProcessor::getTarget()
{
    return HG::Rendering::Base::Texture::DataId;
}
