// HG::Rendering::OpenGL
#include <Common/MeshDataProcessor.hpp>
#include <Common/MeshData.hpp>

// HG::Rendering::Base
#include <Behaviours/Mesh.hpp>

// HG::Utils
#include <Mesh.hpp>

bool HG::Rendering::OpenGL::Common::MeshDataProcessor::setup(HG::Rendering::Base::RenderData* renderData)
{
    auto meshBehaviour = static_cast<HG::Rendering::Base::Behaviours::Mesh*>(renderData);

    if (meshBehaviour->mesh() == nullptr)
    {
        Error() << "Can't setup render without set mesh.";
        return false;
    }

    auto searchResult = m_meshes.find(meshBehaviour->mesh());
    if (searchResult != m_meshes.end())
    {
        delete meshBehaviour->specificData();
        meshBehaviour->setSpecificData(searchResult->second);

        return true;
    }

    Common::MeshData* data = nullptr;

    if ((data = static_cast<MeshData*>(meshBehaviour->specificData())) == nullptr)
    {
        data = new Common::MeshData();
        meshBehaviour->setSpecificData(data);

        m_meshes[meshBehaviour->mesh()] = data;
    }

    if (!data->VAO.is_valid())
    {
        data->VAO = std::move(gl::vertex_array());
    }

    if (!data->VBO.is_valid())
    {
        data->VBO = std::move(gl::buffer());
    }

    if (!data->EBO.is_valid())
    {
        data->EBO = std::move(gl::buffer());
    }

    // Binding vertex array
    data->VAO.bind();

    // Binding vertex buffer object
    data->VBO.bind(GL_ARRAY_BUFFER);

    auto mesh = meshBehaviour->mesh();

    // Loading data into VBO
    data->VBO.set_data(
        mesh->Vertices.size() * sizeof(HG::Utils::Vertex),
        mesh->Vertices.data()
    );

    // Binding element buffer object
    data->EBO.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    data->EBO.set_data(
        mesh->Indices.size() * sizeof(uint32_t),
        mesh->Indices.data()
    );

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

    return true;
}

std::size_t HG::Rendering::OpenGL::Common::MeshDataProcessor::getTarget()
{
    return HG::Rendering::Base::Behaviours::Mesh::Id;
}

bool HG::Rendering::OpenGL::Common::MeshDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto meshData = static_cast<MeshData*>(data->specificData());

    return  meshData == nullptr ||
           !meshData->VAO.is_valid() ||
           !meshData->VBO.is_valid() ||
           !meshData->EBO.is_valid();
}
