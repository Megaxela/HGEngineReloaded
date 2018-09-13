#pragma once

// C++ STL
#include <memory>

// HG::Rendering::Base
#include <RenderBehaviour.hpp> // Required for inheritance

// ALogger
#include <CurrentLogger.hpp>

namespace HG::Utils
{
    class Mesh;
    using MeshPtr = std::shared_ptr<Mesh>;
}

namespace HG::Rendering::Base
{
    class Material;
}

namespace HG::Rendering::Base::Behaviours
{
    /**
     * @brief Class, that describes mesh rendering.
     */
    class Mesh : public RenderBehaviour
    {
    public:

        // Mesh type id
        static constexpr std::size_t Id = 1;

        /**
         * @brief Constructor.
         */
        Mesh();

        /**
         * @brief Method for getting smart pointer
         * to mesh object.
         * @return Smart pointer to mesh or nullptr if
         * no mesh set.
         */
        HG::Utils::MeshPtr mesh() const;

        /**
         * @brief Method for setting mesh object.
         * @param mesh Smart pointer with mesh object.
         */
        void setMesh(HG::Utils::MeshPtr mesh);

        /**
         * @brief Method for getting material for rendering.
         * @return Pointer to material.
         */
        Material* material() const;

        /**
         * @brief Method for setting material for rendering.
         * @param material Pointer to material.
         */
        void setMaterial(Material* material);

    private:

        HG::Utils::MeshPtr m_mesh;
        Material* m_material;
    };
}

