// HG::Utils
#include <HG/Utils/Loaders/StringLoader.hpp>

namespace HG::Utils
{
StringLoader::StringLoader()
{
}

StringLoader::ResultType StringLoader::load(const std::byte* data, std::size_t size)
{
    return std::string((const char*)data, size);
}
} // namespace HG::Utils
