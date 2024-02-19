#include "view_manager.h"

ViewManager::ViewManager() = default;

sf::View ViewManager::centerViewOnEntity(sf::RenderTarget& renderTarget, const Entity& entity)
{
    // keep view centred/centered on entity
    Component::CTransform transformForEntity = entity.getComponent<Component::CTransform>();
    float centreX = getViewCentreForCoordinate(transformForEntity.position->x, LevelManager::activeLevel.width, Crucible::WINDOW_WIDTH, transformForEntity.dimensions.x);
    float centreY = getViewCentreForCoordinate(transformForEntity.position->y, LevelManager::activeLevel.height, Crucible::WINDOW_HEIGHT, transformForEntity.dimensions.y);

    sf::View newView = renderTarget.getView();
    newView.zoom(VIEW_ZOOM_FACTOR);
    newView.setCenter(centreX, centreY);

    renderTarget.setView(newView);
    return newView;
}

float ViewManager::getViewCentreForCoordinate(const float playerCoordinatePosition, const float levelDimension,
                                              const float windowDimensionValue, const float playerDimensionValue) {
    if (playerCoordinatePosition <= windowDimensionValue / 4)
    {
        return windowDimensionValue / 4;
    }

    return playerCoordinatePosition < (playerDimensionValue * levelDimension)
           ? playerCoordinatePosition
           : windowDimensionValue - (windowDimensionValue / 3);
}

std::unordered_map<std::string, sf::VertexArray> ViewManager::getTileVerticesInView(
        const sf::RenderTarget& target,
        const TileLayer& tileLayer,
        const std::vector<TileSet>& tileSets)
{
    std::unordered_map<std::string, sf::VertexArray> tileVerticesInViewPerTileset;
    
    for (const TileSet& tileSet : tileSets)
    {
        sf::VertexArray vArr = sf::VertexArray(sf::Quads);
        //vArr.resize(4 * tileSet.tileCount * tileSet.columns);
        tileVerticesInViewPerTileset.insert({tileSet.path, vArr});
    }

    sf::Vector2f viewCentre = target.getView().getCenter();

    float minOffsetX = viewCentre.x - (Crucible::TILE_SIZE * TOTAL_TILES_VISIBLE_X);
    float maxOffsetX = viewCentre.x + (Crucible::TILE_SIZE * TOTAL_TILES_VISIBLE_X);
    float minOffsetY = viewCentre.y - (Crucible::TILE_SIZE * TOTAL_TILES_VISIBLE_Y);
    float maxOffsetY = viewCentre.y + (Crucible::TILE_SIZE * TOTAL_TILES_VISIBLE_Y);

    for (const TileSet& tileSet : tileSets)
    {
        const sf::VertexArray& tileSetVertexArray = tileLayer.tilesetPathToLevelData.at(tileSet.path);
        for (size_t i = 0; i < tileSetVertexArray.getVertexCount(); i++)
        {
            sf::Vertex vertex = tileSetVertexArray.operator[](i);
            bool vertexInViewOnX = vertex.position.x >= minOffsetX && vertex.position.x < maxOffsetX;
            bool vertexInViewOnY = (vertex.position.y == 0 || vertex.position.y >= minOffsetY) &&
                    vertex.position.y < maxOffsetY;

            if (vertexInViewOnX && vertexInViewOnY)
            {
                tileVerticesInViewPerTileset.at(tileSet.path).append(vertex);
            }
        }
    }

    return tileVerticesInViewPerTileset;
}
