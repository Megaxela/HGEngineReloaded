#include "Loaders/StringLoader.hpp"

UTILS_MODULE_NS::StringLoader::StringLoader()
{

}

UTILS_MODULE_NS::StringLoader::ResultType UTILS_MODULE_NS::StringLoader::load(const std::byte* data, std::size_t size)
{
    return std::string((const char*) data, size);
}
