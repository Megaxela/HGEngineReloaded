// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>
#include <HG/Utils/Logging.hpp>
#include <HG/Utils/Surface.hpp>

// stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace HG::Utils
{
STBImageLoader::STBImageLoader()
{
}

STBImageLoader::ResultType STBImageLoader::load(const std::byte* data, std::size_t size)
{
    // Trying to load image with stbb
    int width;
    int height;
    int bpp;

    auto resultData =
        stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data), static_cast<int>(size), &width, &height, &bpp, 0);

    if (resultData == nullptr)
    {
        HGError("Can't load image from memory, error: {}", stbi_failure_reason());
        return nullptr;
    }

    auto surface = std::make_shared<Surface>([](std::uint8_t* data, int, int, int) { stbi_image_free(data); });

    surface->Data   = resultData;
    surface->Width  = width;
    surface->Height = height;
    surface->Bpp    = bpp;

    return surface;
}
} // namespace HG::Utils
