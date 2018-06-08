#pragma once

#include <RenderBehaviour.hpp>
#include <type_traits>
#include <memory>
#include <CurrentLogger.hpp>
#include <Material.hpp>

namespace UTILS_MODULE_NS
{
    class Mesh;
    using MeshPtr = std::shared_ptr<Mesh>;
}

namespace RENDERING_BASE_MODULE_NS::Behaviours
{
    /**
     * @brief Class, that describes mesh rendering.
     */
    class Mesh : public RenderBehaviour
    {
    public:

        // Mesh type id
        static constexpr std::size_t Id = __COUNTER__;

        /**
         * @brief Constructor.
         */
        Mesh();

        /**
         * @brief Destructor.
         */
        ~Mesh() override;

        /**
         * @brief Method for getting smart pointer
         * to mesh object.
         * @return Smart pointer to mesh or nullptr if
         * no mesh set.
         */
        UTILS_MODULE_NS::MeshPtr mesh() const;

        /**
         * @brief Method for setting mesh object.
         * @param mesh Smart pointer with mesh object.
         */
        void setMesh(UTILS_MODULE_NS::MeshPtr mesh);

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

        UTILS_MODULE_NS::MeshPtr m_mesh;
        Material* m_material;
    };
}

