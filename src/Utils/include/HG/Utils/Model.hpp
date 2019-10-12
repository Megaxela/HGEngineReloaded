#pragma once

// C++ STL
#include <memory>
#include <vector>

namespace HG::Utils
{
class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Model;
using ModelPtr = std::shared_ptr<Model>;

/**
 * @brief Class, that describes model,
 * loaded from resource.
 * It's tree of models. Parent will delete
 * children.
 */
class Model
{
public:
    /**
     * @brief Init constructor.
     * @param mesh Models mesh.
     * @param parent Parent model. If it's
     * nullptr - it's root model.
     */
    explicit Model(Model* parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~Model();

    /**
     * @brief Method for getting models parent model.
     * @return Pointer to parent. If nullptr - it's root
     * model.
     */
    [[nodiscard]] Model* parent() const;

    /**
     * @brief Method for setting parent. Therefore
     * objects owner is changing, so, if new parent is
     * nullptr - you have to delete this object yourself.
     * @param model Pointer to new parent.
     */
    void setParent(Model* model);

    /**
     * @brief Method for getting reference to children.
     * @return Constant reference to children container.
     */
    [[nodiscard]] const std::vector<Model*>& children() const;

    /**
     * @brief Method for adding mesh to model.
     * @param mesh New smart pointer to mesh.
     */
    void addMesh(MeshPtr mesh);

    /**
     * @brief Method for getting container with meshes.
     * @return Container with models meshes.
     */
    [[nodiscard]] const std::vector<MeshPtr>& meshes() const;

private:
    std::vector<MeshPtr> m_meshes;

    Model* m_parent;

    std::vector<Model*> m_children;
};
} // namespace HG::Utils
