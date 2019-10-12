// C++ STL
#include <fstream>

// HG::Core
#include <HG/Core/Logging.hpp>

// HG::Standard
#include <HG/Core/FilesystemResourceAccessor.hpp>

namespace HG::Core
{
FilesystemResourceAccessor::VectorData::VectorData(std::vector<std::byte> data, std::string id) :
    m_data(std::move(data)),
    m_id(std::move(id))
{
}

std::size_t FilesystemResourceAccessor::VectorData::size() const
{
    return m_data.size();
}

const std::byte* FilesystemResourceAccessor::VectorData::data() const
{
    return m_data.data();
}

std::string FilesystemResourceAccessor::VectorData::id() const
{
    return m_id;
}

DataPtr FilesystemResourceAccessor::loadRaw(const std::string& id)
{
    std::ifstream file(id, std::ios::binary);

    if (!file.is_open())
    {
        HGWarning() << "Can't open file \"" << id << "\".";
        return nullptr;
    }

    std::vector<std::byte> data;

    file.seekg(0, std::ios::end);

    auto size = static_cast<size_t>(file.tellg());

    data.resize(size);
    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char*>(data.data()), size);

    return std::make_shared<VectorData>(std::move(data), id);
}
} // namespace HG::Core
