#include <Behaviours/TiledMapRenderer.hpp>
#include <GameObjectBuilder.hpp>
#include <GameObjectCache.hpp>
#include <Loaders/STBImageLoader.hpp>
#include <Behaviours/Mesh.hpp>
#include <Application.hpp>
#include <Texture.hpp>
#include <Shader.hpp>
#include <Scene.hpp>
#include <Mesh.hpp>
#include <filesystem>


HG::Standard::Behaviours::TiledMapRenderer::TiledMapRenderer() :
    m_map(nullptr),
    m_layers(),
    m_mapShader(nullptr),
    m_layerZOffsetCummulative(0),
    m_metersPerPixel(0.01f),
    m_layerZOffset(0.01f)
{

}

HG::Standard::Behaviours::TiledMapRenderer::TiledMapRenderer(HG::Standard::Behaviours::TiledMap *map) :
    m_map(map),
    m_layers(),
    m_mapShader(nullptr),
    m_layerZOffsetCummulative(0),
    m_metersPerPixel(0.01f),
    m_layerZOffset(0.01f)
{

}

HG::Standard::Behaviours::TiledMapRenderer::~TiledMapRenderer()
{
    delete m_mapShader;
}

float HG::Standard::Behaviours::TiledMapRenderer::tiledLayersZOffset() const
{
    return m_layerZOffset;
}

void HG::Standard::Behaviours::TiledMapRenderer::setTiledLayerZOffset(float offset)
{
    m_layerZOffset = offset;
}

void HG::Standard::Behaviours::TiledMapRenderer::setMetersPerPixel(float value)
{
    m_metersPerPixel = value;
}

float HG::Standard::Behaviours::TiledMapRenderer::metersPerPixel() const
{
    return m_metersPerPixel;
}

HG::Standard::Behaviours::TiledMap *HG::Standard::Behaviours::TiledMapRenderer::map() const
{
    return m_map;
}

void HG::Standard::Behaviours::TiledMapRenderer::prepare()
{
    // Clearing gameobjects
    for (auto&& gameObject : m_layers)
    {
        scene()->removeGameObject(gameObject);
    }

    m_layers.clear();

    // Clearing loaded tileset textures
    for (auto&& [name, texture] : m_tilesets)
    {
        delete texture;
    }

    m_tilesets.clear();

    m_layerZOffsetCummulative = 0;

    if (m_map)
    {
        prepareTilesets();

        for (auto&& layer : m_map->rootGroup()->layers)
        {
            prepareLayer(layer, gameObject());
        }
    }
}

void HG::Standard::Behaviours::TiledMapRenderer::onStart()
{
    m_mapShader = new HG::Rendering::Base::Shader;

    // Preparing shader
    m_mapShader->setShaderText(
        R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inTexCoords;

out VS_OUT {
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    vs_out.TexCoords = inTexCoords;
}
#endif

#ifdef FragmentShader
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
} fs_in;

// texture samplers
uniform sampler2D tileset;

void main()
{
    vec4 color = texture(tileset, fs_in.TexCoords).rgba;

    if (color.a > 0.001f)
    {
        FragColor = color;
    }
    else
    {
        discard;
    }
}
#endif
)"
    );

    // todo: Apply validation
    scene()->application()->renderer()->setup(m_mapShader);

    if (m_map)
    {
        prepare();
    }
}

void HG::Standard::Behaviours::TiledMapRenderer::prepareLayer(const HG::Standard::Behaviours::TiledMap::Layer *layer,
                                                               HG::Core::GameObject *parent)
{
    switch (layer->type)
    {
    case TiledMap::Layer::Type::Tile:
        prepareTileLayer(
            dynamic_cast<const TiledMap::TileLayer *>(layer),
            parent
        );
        break;
    case TiledMap::Layer::Type::Group:
        prepareGroupLayer(
            dynamic_cast<const TiledMap::Group *>(layer),
            parent
        );
        break;
    case TiledMap::Layer::Type::Object: // fall through
    case TiledMap::Layer::Type::Image:
        break;
    }
}

void HG::Standard::Behaviours::TiledMapRenderer::prepareTilesets()
{
    const auto& tilesets = m_map->tilesets();

    for (auto&& tileset : tilesets)
    {
        // Getting path from id
        std::filesystem::path mapPath = m_map->properties().mapPath;

        // Trying to load texture

//        if (tilesetData == nullptr)
//        {
//            // todo: Add usage of replace texture
//            Error() << "Can't load tileset \"" << tileset->name << "\" texture at \"" << tileset->path << "\".";
//            m_tilesets[tileset->path] = nullptr;
//        }

        // Making texture
        auto texture = new HG::Rendering::Base::Texture(
            scene()
                ->application()
                ->resourceManager()
                ->load<HG::Utils::STBImageLoader>(
                    mapPath.parent_path() / std::filesystem::path(tileset->path)
                )
        );

        // todo: Add validation, when it will appear at `setup` method.
        scene()
            ->application()
            ->renderer()
            ->setup(texture);

        m_tilesets[tileset->path] = texture;
    }
}

void HG::Standard::Behaviours::TiledMapRenderer::prepareGroupLayer(const HG::Standard::Behaviours::TiledMap::Group *groupLayer,
                                                                    HG::Core::GameObject *parent)
{
    // Creating game object

    auto pixelScale = 0.01f;

    HG::Core::GameObject* newParentGameObject =
        HG::Core::GameObjectBuilder()
            .setName(groupLayer->name)
            .setParent(parent);

    scene()->addGameObject(newParentGameObject);

    auto pos = newParentGameObject->transform()->localPosition();

    pos.x =  groupLayer->offset.x * pixelScale;
    pos.y = -groupLayer->offset.y * pixelScale;

    newParentGameObject->transform()->setLocalPosition(pos);

    for (auto&& layer : groupLayer->layers)
    {
        prepareLayer(layer, newParentGameObject);
    }
}

void HG::Standard::Behaviours::TiledMapRenderer::prepareTileLayer(const HG::Standard::Behaviours::TiledMap::TileLayer *tileLayer,
                                                                  HG::Core::GameObject *parent)
{
    // Creating game object
    HG::Core::GameObject* layerGameObject =
        HG::Core::GameObjectBuilder()
            .setName(tileLayer->name)
            .setParent(parent);

    scene()->addGameObject(layerGameObject);

    auto pos = layerGameObject->transform()->localPosition();

    pos.x =  tileLayer->offset.x * m_metersPerPixel;
    pos.y = -tileLayer->offset.y * m_metersPerPixel;
    pos.z =  m_layerZOffsetCummulative;

    // todo: Add configuration
    m_layerZOffsetCummulative += m_layerZOffset;

    layerGameObject->transform()->setLocalPosition(pos);

    std::map<
        HG::Rendering::Base::Texture*,
        HG::Utils::MeshPtr
    > rendererMeshInfo;

    glm::ivec2 tilePointer = {-1, 0}; // Initial position

    glm::vec2 worldMapTileSize = {
        m_map->properties().tileSize.x * m_metersPerPixel,
        m_map->properties().tileSize.y * m_metersPerPixel
    };

    glm::vec2 worldLayerSize = {
        m_map->properties().size.x * worldMapTileSize.x,
        m_map->properties().size.y * worldMapTileSize.y
    };

    // todo: Add configuration
    constexpr float tileOverlay = 0.0002f;

    for (auto&& tile : tileLayer->tiles)
    {
        // Moving tile pointer to next tile
        tilePointer.x += 1;

        if (tilePointer.x >= tileLayer->size.x)
        {
            tilePointer.y += 1;
            tilePointer.x = 0;
        }

        if (tile == 0)
        {
            continue;
        }

        // Getting invert/rotate mask
        auto decodedTile =
            HG::Standard::Behaviours::TiledMap::TileLayer::decodeTile(tile);

        // Getting required tileset
        auto tilesetIter = std::find_if(
            m_map->tilesets().begin(),
            m_map->tilesets().end(),
            [decodedTile](HG::Standard::Behaviours::TiledMap::Tileset* tileset) -> bool
            {
                return decodedTile.actualTile >= tileset->firstGID &&
                       decodedTile.actualTile <  tileset->firstGID + tileset->tileCount;
            }
        );

        // If tileset was not found
        if (tilesetIter == m_map->tilesets().end())
        {
            Error() << "Tileset for GID " << decodedTile.actualTile << " was not found.";
        }

        // Calculating tile position at tileset
        auto index = decodedTile.actualTile - (*tilesetIter)->firstGID;

        glm::ivec2 tileIndex = {
            index % (*tilesetIter)->columns,
            index / (*tilesetIter)->columns
        };

        glm::vec2 spacingUV = {
            float((*tilesetIter)->spacing) / (*tilesetIter)->imageSize.x,
            float((*tilesetIter)->spacing) / (*tilesetIter)->imageSize.y
        };

        glm::vec2 tileUV = {
            1.0f / (*tilesetIter)->imageSize.x * (*tilesetIter)->tileSize.x,
            1.0f / (*tilesetIter)->imageSize.y * (*tilesetIter)->tileSize.y
        };

        glm::vec2 topLeftUV = {
             tileIndex.x      * tileUV.x + spacingUV.x * tileIndex.x,
             tileIndex.y      * tileUV.y + spacingUV.y * tileIndex.y
        };

        glm::vec2 topRightUV = {
            (tileIndex.x + 1) * tileUV.x + spacingUV.x * tileIndex.x,
             tileIndex.y      * tileUV.y + spacingUV.y * tileIndex.y
        };

        glm::vec2 botLeftUV = {
             tileIndex.x      * tileUV.x + spacingUV.x * tileIndex.x,
            (tileIndex.y + 1) * tileUV.y + spacingUV.y * tileIndex.y
        };

        glm::vec2 botRightUV = {
            (tileIndex.x + 1) * tileUV.x + spacingUV.x * tileIndex.x,
            (tileIndex.y + 1) * tileUV.y + spacingUV.y * tileIndex.y
        };

        // todo: Add flipping implementation

        if (decodedTile.verticallyFlipped)
        {
            std::swap(topLeftUV .y, botLeftUV .y);
            std::swap(topRightUV.y, botRightUV.y);
        }

        if (decodedTile.horizontallyFlipped)
        {
            std::swap(topLeftUV.x, topRightUV.x);
            std::swap(botLeftUV.x, botRightUV.x);
        }

        if (decodedTile.diagonallyFlipped)
        {
            std::swap(topLeftUV, botRightUV);
        }

        // Getting texture for tileset
        auto textureIter = m_tilesets.find((*tilesetIter)->path);

        if (textureIter == m_tilesets.end())
        {
            // Texture was not found. Log and skip this.
            // todo: Get advantage of fallback texture.
            Error() << "Can't find texture for tileset \"" << (*tilesetIter)->name << "\".";
            continue;
        }

        // Getting mesh by texture, or creating it
        auto meshIter = rendererMeshInfo.find(textureIter->second);

        // If mesh was not found - create it.
        if (meshIter == rendererMeshInfo.end())
        {
            meshIter = rendererMeshInfo.insert_or_assign(
                textureIter->second,
                std::make_shared<HG::Utils::Mesh>()
            ).first;
        }

        /*
         * (*tilesetIter)->tileSize.x
         * (*tilesetIter)->tileSize.y
         */
        glm::vec2 worldTilesetTileSize = {
            (*tilesetIter)->tileSize.x * m_metersPerPixel,
            (*tilesetIter)->tileSize.y * m_metersPerPixel
        };

        // Getting tile position
        // Top left
        meshIter->second->Vertices.push_back(
            {{
                 -(worldLayerSize.x / 2) + ( worldMapTileSize.x *  tilePointer.x                              ) - tileOverlay,
                  (worldLayerSize.y / 2) - ( worldMapTileSize.y * (tilePointer.y + 1) ) + worldTilesetTileSize.y + tileOverlay,
                 0
             },
             {topLeftUV.x, topLeftUV.y}}
        );

        // Bottom left
        meshIter->second->Vertices.push_back(
            {{
                -(worldLayerSize.x / 2) + (worldMapTileSize.x *  tilePointer.x    )                             - tileOverlay,
                 (worldLayerSize.y / 2) - (worldMapTileSize.y * (tilePointer.y + 1)                           ) - tileOverlay,
                 0
             },
             {botLeftUV.x, botLeftUV.y}}
        );

        // Top right
        meshIter->second->Vertices.push_back(
            {{
                -(worldLayerSize.x / 2) + (worldMapTileSize.x *  tilePointer.x     ) + worldTilesetTileSize.x + tileOverlay,
                 (worldLayerSize.y / 2) - (worldMapTileSize.y * (tilePointer.y + 1)) + worldTilesetTileSize.y + tileOverlay,
                0
            },
            {topRightUV.x, topRightUV.y}}
        );

        // Bottom right
        meshIter->second->Vertices.push_back(
            {{
                -(worldLayerSize.x / 2) + (worldMapTileSize.x *  tilePointer.x     ) + worldTilesetTileSize.x + tileOverlay,
                 (worldLayerSize.y / 2) - (worldMapTileSize.y * (tilePointer.y + 1))                          - tileOverlay,
                0
             },
             {botRightUV.x, botRightUV.y}}
        );

        uint32_t firstIndex = 0;

        if (!meshIter->second->Indices.empty())
        {
            firstIndex = meshIter->second->Indices[meshIter->second->Indices.size() - 4] + 1;
        }

        meshIter->second->Indices.push_back(firstIndex + 1); // Bottom left
        meshIter->second->Indices.push_back(firstIndex + 2); // Top right
        meshIter->second->Indices.push_back(firstIndex + 3); // Bottom right
        meshIter->second->Indices.push_back(firstIndex + 1); // Bottom left
        meshIter->second->Indices.push_back(firstIndex + 0); // Top left
        meshIter->second->Indices.push_back(firstIndex + 2); // Top right
    }

    // Adding render behaviours
    for (auto&& [texture, mesh] : rendererMeshInfo)
    {
        auto material = new HG::Rendering::Base::Material;

        material->setShader(m_mapShader);
        material->set("tileset", texture);

        auto meshRenderer = new HG::Rendering::Base::Behaviours::Mesh;

        meshRenderer->setMesh(mesh);
        meshRenderer->setMaterial(material);

        scene()->application()->renderer()->setup(meshRenderer);

        layerGameObject->addBehaviour(meshRenderer);
    }
}
