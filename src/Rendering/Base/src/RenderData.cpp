// HG::Rendering::Base
#include <RenderData.hpp>
#include <RenderSpecificData.hpp>

HG::Rendering::Base::RenderData::RenderData(std::size_t type) :
    m_data(nullptr),
    m_type(type)
{

}

HG::Rendering::Base::RenderData::~RenderData()
{
    delete m_data;
    m_data = nullptr;
}

HG::Rendering::Base::RenderSpecificData* HG::Rendering::Base::RenderData::specificData()
{
    return m_data;
}

void HG::Rendering::Base::RenderData::setSpecificData(HG::Rendering::Base::RenderSpecificData* data)
{
    m_data = data;
}

std::size_t HG::Rendering::Base::RenderData::dataType() const
{
    return m_type;
}

void HG::Rendering::Base::RenderData::invalidate()
{
    if (m_data == nullptr)
    {
        return;
    }

    m_data->Valid = false;
}
