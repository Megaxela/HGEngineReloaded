#include "Surface.hpp"
#include "stb_image.h"

HG::Utils::Surface::Surface(FreeFunction f) :
    Data(nullptr),
    Width(0),
    Height(0),
    Bpp(0),
    m_freeFunction(f)
{

}

HG::Utils::Surface::~Surface()
{
    if (m_freeFunction && Data)
    {
        m_freeFunction(Data, Width, Height, Bpp);
    }
}
