#pragma once

// C++ STL
#include <memory>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderBehaviour.hpp> // Required for inheritance

// HG::Utils
#include <HG/Utils/StringTools.hpp>

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
    class Mesh : public HG::Rendering::Base::RenderBehaviour
    {
    public:

        // Mesh type id
        static constexpr std::size_t RenderBehaviourId = HG::Utils::StringTools::hash("RenderBehaviour::Mesh");

        /**
         * @brief Constructor.
         */
        explicit Mesh(HG::Utils::MeshPtr mesh=nullptr,
                      HG::Rendering::Base::Material* material=nullptr);

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
        HG::Rendering::Base::Material* material() const;

        /**
         * @brief Method for setting material for rendering.
         * @param material Pointer to material.
         */
        void setMaterial(HG::Rendering::Base::Material* material);

    private:

        HG::Utils::MeshPtr m_mesh;
        HG::Rendering::Base::Material* m_material;
    };
}

