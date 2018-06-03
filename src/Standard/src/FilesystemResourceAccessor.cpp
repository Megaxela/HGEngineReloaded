#include <fstream>
#include <CurrentLogger.hpp>
#include "FilesystemResourceAccessor.hpp"

STD_MODULE_NS::FilesystemResourceAccessor::VectorData::VectorData(std::vector<std::byte> data) :
    m_data(std::move(data))
{

}

std::size_t STD_MODULE_NS::FilesystemResourceAccessor::VectorData::size() const
{
    return m_data.size();
}

const std::byte *STD_MODULE_NS::FilesystemResourceAccessor::VectorData::data() const
{
    return m_data.data();
}

::CORE_MODULE_NS::ResourceAccessor::DataPtr
STD_MODULE_NS::FilesystemResourceAccessor::loadRaw(const std::string &id)
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

    return std::make_shared<VectorData>(std::move(data));
}


