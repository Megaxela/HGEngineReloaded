#pragma once

#include <memory>
#include <map>
#include <functional>
#include <ResourceAccessor.hpp>

namespace UTILS_MODULE_NS
{
    class Mesh;
    using MeshPtr = std::shared_ptr<Mesh>;
}

namespace STD_MODULE_NS::MeshLoaders
{
    /**
     * @brief Class, for loading raw OBJ
     * data to mesh object.
     */
    class OBJ
    {
    public:

        /**
         * @brief Constructor. (This classes
         * has to be default constructable)
         */
        OBJ();

        /**
         * @brief Method for loading mesh from raw data.
         * @param data Raw data.
         * @return Smart pointer to mesh.
         */
        ::UTILS_MODULE_NS::MeshPtr load(::CORE_MODULE_NS::ResourceAccessor::DataPtr data);

    private:

        static std::map<
            std::string_view,
            std::function<
                void(
                    ::UTILS_MODULE_NS::MeshPtr&,       // mesh ptr
                    std::string_view::const_iterator&, // iterator
                    std::string_view&                  // line
                )
            >
        > m_tokenProcessors;
    };
}

