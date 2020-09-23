#pragma once

// HG::System::PC
#include <HG/System/PC/Base/ContextInitializer.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderingPipeline.hpp>

// C++ STL
#include <functional>
#include <memory>

namespace HG::System::PC
{
    /**
     * @brief Class, that's responsible for construction
     * of backend specific initializers.
     */
    class InitializerFabric
    {
    public:
        /**
         * @brief Constructor.
         */
        InitializerFabric();

        /**
         * @brief Method for constructing backend specific initializer.
         * It may return nullptr if no initializer available for provided backend.
         */
        [[nodiscard]] std::shared_ptr<HG::System::PC::Base::ContextInitializer> build(
            HG::Rendering::Base::RenderingPipeline::Backend backend
        ) const;

    private:
        template<typename T>
        void addInitializer()
        {
            m_builders[T::SupportedBackend] = []()
            {
                return std::make_shared<T>();
            };
        }

        using BuildFunction = std::function<
            std::shared_ptr<HG::System::PC::Base::ContextInitializer>()
        >;

        std::unordered_map<
            HG::Rendering::Base::RenderingPipeline::Backend,
            BuildFunction
        > m_builders;
    };
}