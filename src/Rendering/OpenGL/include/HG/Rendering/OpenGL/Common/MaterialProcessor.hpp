#pragma once

// C++ STL
#include <string>

namespace HG::Core
{
class Application;
}

namespace HG::Rendering::Base
{
class Material;
class MaterialValue;
} // namespace HG::Rendering::Base

namespace HG::Rendering::OpenGL::Common
{
class ShaderData;

/**
 * @brief Class, that describes class, that provides
 * sample method for operating with material.
 * If some renderer uses material it can inherit
 * this class and use it's methods.
 */
class MaterialProcessor
{
public:
    /**
     * @brief Constructor.
     */
    MaterialProcessor();

protected:
    /**
     * @brief Method for using shader provided in material.
     * @param material Pointer to material.
     */
    void useMaterial(HG::Core::Application* application, HG::Rendering::Base::Material* material);

    /**
     * @brief Method for applying all material values to
     * it's shader's uniforms.
     * @param application Pointer to parent application.
     * @param material Pointer to material.
     * @param guarantee If true - applier will wait until all async calls.
     */
    void applyMaterialUniforms(HG::Core::Application* application,
                               HG::Rendering::Base::Material* material,
                               bool guarantee = false);

    /**
     * @brief Method for applying materials value to
     * shader's named uniform.
     * @param shaderData Pointer to shader data.
     * @param name Name.
     * @param value Value.
     * @prarm textureNumber Number of available texture.
     * If texture was used - this value will change.
     * @param guarantee If true - applier will wait until all async calls.
     */
    void setShaderUniform(HG::Core::Application* application,
                          HG::Rendering::OpenGL::Common::ShaderData* shaderData,
                          const std::string& name,
                          const HG::Rendering::Base::MaterialValue& value,
                          std::uint32_t& textureNumber,
                          bool guarantee = false);

private:
};
} // namespace HG::Rendering::OpenGL::Common
