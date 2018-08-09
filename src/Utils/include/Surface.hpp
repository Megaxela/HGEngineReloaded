#pragma once

#include <cstdint>
#include <memory>

namespace HG::Utils
{
    /**
     * @brief Class, that describes RAM loaded
     * bitmap.
     */
    class Surface
    {
    public:

        using FreeFunction = void(*)(uint8_t*, int, int, int);

        // Properties
        uint8_t* Data;
        int Width;
        int Height;
        int Bpp;

        // Disable copying
        Surface(const Surface& s) = delete;
        Surface& operator=(const Surface&) = delete;

        /**
         * @brief Constructor.
         * Constructs empty surface.
         */
        explicit Surface(FreeFunction f=nullptr);

        /**
         * @brief Destructor, that calls free function
         * if it's defined.
         */
        ~Surface();

    private:
        FreeFunction m_freeFunction;
    };

    using SurfacePtr = std::shared_ptr<Surface>;
}

