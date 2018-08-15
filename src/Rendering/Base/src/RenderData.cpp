#include <RenderData.hpp>

HG::Rendering::Base::RenderData::RenderData(std::size_t type) :
    m_data(nullptr),
    m_type(type)
{

}

HG::Rendering::Base::RenderData::~RenderData()
{
    clearSpecificData();
}

void HG::Rendering::Base::RenderData::clearSpecificData()
{
    delete m_data;
    m_data = nullptr;
}

std::size_t HG::Rendering::Base::RenderData::dataType() const
{
    return m_type;
}
