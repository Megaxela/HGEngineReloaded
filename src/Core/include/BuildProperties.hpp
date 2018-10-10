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

    /**
     * @brief Constexpr function for checking is
     * HG::Rendering::Base module is linked.
     */
    constexpr bool hasRenderingBase()
    {
#ifdef HG_HAS_RENDERING_BASE
        return true;
#else
        return false;
#endif
    }

    /**
     * @brief Constexpr function for checking is
     * HG::Rendering::OpenGL module is linked.
     */
    constexpr bool hasRenderingOpenGL()
    {
#ifdef HG_HAS_RENDERING_OGL
        return true;
#else
        return false;
#endif
    }

    /**
     * @brief Constexpr function for checking is
     * HG::Standard module is linked.
     */
    constexpr bool hasStandard()
    {
#ifdef HG_HAS_STANDARD
        return true;
#else
        return false;
#endif
    }

    /**
     * @brief Constexpr function for checking is
     * HG::Utils module is linked.
     */
    constexpr bool hasUtils()
    {
#ifdef HG_HAS_UTILS
        return true;
#else
        return false;
#endif
    }

    /**
     * @brief Constexpr function for checking is
     * HG::Physics::Base module is linked.
     */
    constexpr bool hasPhysicsBase()
    {
#ifdef HG_HAS_PHYSICS_BASE
        return true;
#else
        return false;
#endif
    }

    /**
     * @brief Constexpr function for checking is
     * HG::Physics::PlayRho module is linked.
     * @return
     */
    constexpr bool hasPhysicsPlayRho()
    {
#ifdef HG_HAS_PHYSICS_PLAYRHO
        return true;
#else
        return false;
#endif
    }
};

