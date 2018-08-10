#include <fstream>
#include <CurrentLogger.hpp>
#include "FilesystemResourceAccessor.hpp"

HG::Standard::FilesystemResourceAccessor::VectorData::VectorData(std::vector<std::byte> data, std::string id) :
    m_data(std::move(data)),
    m_id(std::move(id))
{

}

std::size_t HG::Standard::FilesystemResourceAccessor::VectorData::size() const
{
    return m_data.size();
}

const std::byte *HG::Standard::FilesystemResourceAccessor::VectorData::data() const
{
    return m_data.data();
}

std::string HG::Standard::FilesystemResourceAccessor::VectorData::id() const
{
    return m_id;
}

HG::Core::ResourceAccessor::DataPtr
HG::Standard::FilesystemResourceAccessor::loadRaw(const std::string &id)
{
    std::ifstream file(id, std::ios::binary);

    if (!file.is_open())
    {
        Warning() << "Can't open file \"" << id << "\".";
        return nullptr;
    }

    std::vector<std::byte> data;

    file.seekg(0, std::ios::end);

    auto size = static_cast<size_t>(file.tellg());

    data.resize(size);
    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char *>(data.data()), size);

    return std::make_shared<VectorData>(std::move(data), id);
}


