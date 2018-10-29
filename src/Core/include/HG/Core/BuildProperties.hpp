#pragma once

namespace HG::Core::BuildProperties
{
    /**
     * @brief Constexpr function for checking is
     * debug build enabled.
     */
    constexpr bool isDebug()
    {
#ifdef NDEBUG
        return false;
#else
        return true;
#endif
    }
};

