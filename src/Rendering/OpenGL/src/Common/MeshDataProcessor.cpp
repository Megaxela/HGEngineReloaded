// HG::Core
#include <HG/Core/Application.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/MeshData.hpp>
#include <HG/Rendering/OpenGL/Common/MeshDataProcessor.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/Mesh.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>
#include <HG/Utils/Mesh.hpp>

namespace HG::Rendering::OpenGL::Common
{
bool MeshDataProcessor::setup(HG::Rendering::Base::RenderData* renderData, bool guarantee)
{
    auto meshBehaviour = dynamic_cast<HG::Rendering::Base::Behaviours::Mesh*>(renderData);

    if (meshBehaviour == nullptr)
    {
        HGError("Got non mesh behaviour render data in mesh behaviour data processor, types are corrupted");
        exit(-1);
    }

    if (meshBehaviour->mesh() == nullptr)
    {
        HGError("Can't setup render without set mesh");
        return false;
    }

    Common::MeshData* data = nullptr;

    if ((data = meshBehaviour->castSpecificDataTo<MeshData>()) == nullptr)
    {
        data = new (application()->resourceCache()) Common::MeshData();
        meshBehaviour->setSpecificData(data);
    }

    if (data->VAO.id() == gl::invalid_id)
    {
        data->VAO = std::move(gl::vertex_array());
    }

    if (data->VBO.id() == gl::invalid_id)
    {
        data->VBO = std::move(gl::buffer());
    }

    if (data->EBO.id() == gl::invalid_id)
    {
        data->EBO = std::move(gl::buffer());
    }

    // Binding vertex array
    data->VAO.bind();

    // Binding vertex buffer object
    data->VBO.bind(GL_ARRAY_BUFFER);

    auto mesh = meshBehaviour->mesh();

    // Loading data into VBO
    data->VBO.set_data(mesh->Vertices.size() * sizeof(HG::Utils::Vertex), mesh->Vertices.data());

    // Binding element buffer object
    data->EBO.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    data->EBO.set_data(mesh->Indices.size() * sizeof(std::uint32_t), mesh->Indices.data());

    // Binding vertex buffer
    data->VAO.set_vertex_buffer(0, data->VBO, 0, sizeof(HG::Utils::Vertex));
    data->VAO.set_vertex_buffer(1, data->VBO, 0, sizeof(HG::Utils::Vertex));
    data->VAO.set_vertex_buffer(2, data->VBO, 0, sizeof(HG::Utils::Vertex));

    // Enabling attributes
    data->VAO.set_attribute_enabled(0, true);
    data->VAO.set_attribute_enabled(1, true);
    data->VAO.set_attribute_enabled(2, true);
    data->VAO.set_attribute_enabled(3, true);
    data->VAO.set_attribute_enabled(4, true);

    // Setting
    data->VAO.set_attribute_format(0, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, position)));
    data->VAO.set_attribute_format(1, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, normal)));
    data->VAO.set_attribute_format(2, 2, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, uv)));
    data->VAO.set_attribute_format(3, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, tangent)));
    data->VAO.set_attribute_format(4, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, bitangent)));

    data->Valid = true;
    data->Count = static_cast<std::uint32_t>(mesh->Indices.size());

    return true;
}

std::size_t MeshDataProcessor::getTarget()
{
    return HG::Rendering::Base::Behaviours::Mesh::RenderBehaviourId;
}

bool MeshDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto meshData = data->castSpecificDataTo<MeshData>();

    return meshData == nullptr || meshData->VAO.id() == gl::invalid_id || meshData->VBO.id() == gl::invalid_id ||
           meshData->EBO.id() == gl::invalid_id || !meshData->Valid;
}
} // namespace HG::Rendering::OpenGL::Common
