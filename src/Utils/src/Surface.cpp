// HG::Utils
#include <HG/Utils/Surface.hpp>

namespace HG::Utils
{
Surface::Surface(FreeFunction f) : Data(nullptr), Width(0), Height(0), Bpp(0), m_freeFunction(f)
{
}

Surface::~Surface()
{
    if (m_freeFunction && Data)
    {
        m_freeFunction(Data, Width, Height, Bpp);
    }
}
} // namespace HG::Utils
