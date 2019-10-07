// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>
#include <HG/Utils/Surface.hpp>

// ALogger
#include <CurrentLogger.hpp>

// stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

HG::Utils::STBImageLoader::STBImageLoader()
{
}

HG::Utils::STBImageLoader::ResultType HG::Utils::STBImageLoader::load(const std::byte* data, std::size_t size)
{
    // Trying to load image with stbb
    int width;
    int height;
    int bpp;

    auto resultData =
        stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data), static_cast<int>(size), &width, &height, &bpp, 0);

    if (resultData == nullptr)
    {
        Error() << "Can't load image from memory. Error: " << stbi_failure_reason();
        return nullptr;
    }

    auto surface = std::make_shared<Surface>([](uint8_t* data, int, int, int) { stbi_image_free(data); });

    surface->Data   = resultData;
    surface->Width  = width;
    surface->Height = height;
    surface->Bpp    = bpp;

    return surface;
}
