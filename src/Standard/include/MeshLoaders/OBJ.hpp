#pragma once

#include <memory>
#include <map>
#include <functional>
#include <ResourceAccessor.hpp>

namespace HG::Utils
{
    class Mesh;
    using MeshPtr = std::shared_ptr<Mesh>;
}

namespace HG::Standard::MeshLoaders
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
        ::HG::Utils::MeshPtr load(::HG::Core::ResourceAccessor::DataPtr data);

    private:

        static std::map<
            std::string_view,
            std::function<
                void(
                    ::HG::Utils::MeshPtr&,       // mesh ptr
                    std::string_view::const_iterator&, // iterator
                    std::string_view&                  // line
                )
            >
        > m_tokenProcessors;
    };
}

