#include <HG/Utils/Interfaces/Initializable.hpp>

HG::Utils::Interfaces::Initializable::Initializable() :
    m_initialized(false)
{

}

HG::Utils::Interfaces::Initializable::~Initializable()
{
    deinit();
}

void HG::Utils::Interfaces::Initializable::init()
{
    if (!m_initialized)
    {
        onInit();
        m_initialized = true;
    }
}

void HG::Utils::Interfaces::Initializable::deinit()
{
    if (m_initialized)
    {
        onDeinit();
        m_initialized = false;
    }
}

bool HG::Utils::Interfaces::Initializable::isInitialized() const
{
    return m_initialized;
}

void HG::Utils::Interfaces::Initializable::onInit()
{

}

void HG::Utils::Interfaces::Initializable::onDeinit()
{

}
