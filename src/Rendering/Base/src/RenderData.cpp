// HG::Rendering::Base
#include <HG/Rendering/Base/RenderData.hpp>
#include <HG/Rendering/Base/RenderSpecificData.hpp>

namespace HG::Rendering::Base
{
RenderData::RenderData(std::size_t type) : m_data(nullptr), m_type(type)
{
}

RenderData::~RenderData()
{
    delete m_data;
    m_data = nullptr;
}

RenderSpecificData* RenderData::specificData() const
{
    return m_data;
}

void RenderData::setSpecificData(RenderSpecificData* data)
{
    m_data = data;
}

std::size_t RenderData::dataType() const
{
    return m_type;
}

void RenderData::invalidate()
{
    if (m_data == nullptr)
    {
        return;
    }

    m_data->Valid = false;
}
} // namespace HG::Rendering::Base
