#pragma once


#include <Behaviour.hpp>
#include "TiledMap.hpp"

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Class, that describes behaviour, that prepares
     * tiled map rendering.
     * This behaviour will create child game objects, that will
     * render different tile layers. Layer game object can have
     * several mesh renderers, because of different tile sets.
     * @todo Add layer opacity support.
     * @todo Add image layer support.
     */
    class TiledMapRenderer : public CORE_MODULE_NS::Behaviour
    {
    public:

        /**
         * @brief Default constructor.
         */
        TiledMapRenderer();

        /**
         * @brief Constructor from map behaviour.
         * @param map Pointer to tiled map behaviour.
         */
        explicit TiledMapRenderer(TiledMap* map);

        /**
         * @brief Method for preparing map for rendering.
         */
        void prepare();

        /**
         * @brief Method for setting world with of 1 pixel.
         * Used in preparing. Default: 0.01 meter (1 cm)
         * @param value Meters per pixel.
         */
        void setMetersPerPixel(float value);

        /**
         * @brief Method for getting world width of 1 pixel.
         * Defulat: 0.01 meter (1 cm)
         * @return Meters per pixel.
         */
        float metersPerPixel() const;

        /**
         * @brief Method for getting associated tiled map.
         * @return Pointer to map.
         */
        TiledMap* map() const;

    protected:

        /**
         * @brief On start, behaviour will execute `prepare` method
         * if map was defined.
         */
        void onStart() override;

    private:

        /**
         * @brief Method, that proceed layer with parent game object.
         * @param layer Layer to proceed.
         * @param parent Parent for targets.
         */
        void prepareLayer(const TiledMap::Layer* layer, CORE_MODULE_NS::GameObject* parent);

        /**
         * @brief Method for getting and loading tiled map
         * tilesets into textures.
         */
        void prepareTilesets();

        // todo: Add commentary
        void prepareTileLayer(const TiledMap::TileLayer* tileLayer, CORE_MODULE_NS::GameObject* parent);

        // todo: Add commentary
        void prepareGroupLayer(const TiledMap::Group* groupLayer, CORE_MODULE_NS::GameObject* parent);

        // Actual map for rendering
        TiledMap* m_map;

        // Layers
        std::vector<CORE_MODULE_NS::GameObject*> m_layers;

        // Loaded tileset textures.
        std::map<std::string, RENDERING_BASE_MODULE_NS::Texture*> m_tilesets;
        
        // Map rendering shader
        RENDERING_BASE_MODULE_NS::Shader m_mapShader;

        float m_layerZOffset;

        float m_metersPerPixel;
    };
}


