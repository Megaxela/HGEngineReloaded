#include "Surface.hpp"
#include "stb_image.h"

UTILS_MODULE_NS::Surface::Surface(FreeFunction f) :
    Data(nullptr),
    Width(0),
    Height(0),
    Bpp(0),
    m_freeFunction(f)
{

}

UTILS_MODULE_NS::Surface::~Surface()
{
    if (m_freeFunction && Data)
    {
        m_freeFunction(Data, Width, Height, Bpp);
    }
}
