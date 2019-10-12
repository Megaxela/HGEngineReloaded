// HG::Utils
#include <HG/Utils/Interfaces/Initializable.hpp>

namespace HG::Utils::Interfaces
{
Initializable::Initializable() : m_initialized(false)
{
}

Initializable::~Initializable()
{
    deinit();
}

void Initializable::init()
{
    if (!m_initialized)
    {
        onInit();
        m_initialized = true;
    }
}

void Initializable::deinit()
{
    if (m_initialized)
    {
        onDeinit();
        m_initialized = false;
    }
}

bool Initializable::isInitialized() const
{
    return m_initialized;
}

void Initializable::onInit()
{
}

void Initializable::onDeinit()
{
}
} // namespace HG::Utils::Interfaces
