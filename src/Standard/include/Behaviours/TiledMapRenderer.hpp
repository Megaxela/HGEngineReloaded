#pragma once


#include <Behaviour.hpp>
#include "TiledMap.hpp"

namespace HG::Standard::Behaviours
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
    class TiledMapRenderer : public HG::Core::Behaviour
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

        /**
         * @brief Method for getting tiled layer offset.
         * Default: 0.01 meter.
         * @return Offset.
         */
        float tiledLayersZOffset() const;

        /**
         * @brief Method for setting layer offset.
         * Default: 0.01 meter.
         * @param offset Offset.
         */
        void setTiledLayerZOffset(float offset);

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
        void prepareLayer(const TiledMap::Layer* layer, HG::Core::GameObject* parent);

        /**
         * @brief Method for getting and loading tiled map
         * tilesets into textures.
         */
        void prepareTilesets();

        // todo: Add commentary
        void prepareTileLayer(const TiledMap::TileLayer* tileLayer, HG::Core::GameObject* parent);

        // todo: Add commentary
        void prepareGroupLayer(const TiledMap::Group* groupLayer, HG::Core::GameObject* parent);

        // Actual map for rendering
        TiledMap* m_map;

        // Layers
        std::vector<HG::Core::GameObject*> m_layers;

        // Loaded tileset textures.
        std::map<std::string, HG::Rendering::Base::Texture*> m_tilesets;
        
        // Map rendering shader
        HG::Rendering::Base::Shader m_mapShader;

        float m_layerZOffsetCummulative;

        float m_metersPerPixel;

        float m_layerZOffset;
    };
}


