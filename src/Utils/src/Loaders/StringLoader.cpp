#include "Loaders/StringLoader.hpp"

HG::Utils::StringLoader::StringLoader()
{

}

HG::Utils::StringLoader::ResultType HG::Utils::StringLoader::load(const std::byte* data, std::size_t size)
{
    return std::string((const char*) data, size);
}
