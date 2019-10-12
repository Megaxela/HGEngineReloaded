#pragma once

// C++ STL
#include <cstdio>

// HG::Core
#include <HG/Core/Behaviour.hpp> // Required for inheritance

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderData.hpp> // Required for inheritance

// HG::Utils
#include <HG/Utils/StringTools.hpp>

namespace HG::Rendering::Base
{
/**
 * @brief Class, that describes cross pipeline
 * behaviour for rendering.
 */
class RenderBehaviour
    : public HG::Core::Behaviour
    , public RenderData
{
public:
    static constexpr std::size_t RenderDataId = HG::Utils::StringTools::hash("RenderData::RenderBehaviour");

    /**
     * @brief Constructor.
     * @param type Derived class `typeid(*this).hash_code()` value.
     */
    explicit RenderBehaviour(std::size_t type);

    /**
     * @brief Method for getting behaviour type.
     * @return Real class `typeid(T).hash_code()` value.
     */
    [[nodiscard]] std::size_t renderBehaviourType() const;

protected:
    // Restrict to override this HG::Core::Behaviour methods

    void onStart() final;

    void onUpdate() final;

    void onFixedUpdate() final;

private:
    std::size_t m_type;
};
} // namespace HG::Rendering::Base
