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

    for (const TileSet& tileSet : tileSets)
    {
        const sf::VertexArray& tileSetVertexArray = tileLayer.tilesetPathToLevelData.at(tileSet.path);
        for (size_t i = 0; i < tileSetVertexArray.getVertexCount(); i+=4)
        {
            sf::Vertex vertexA = tileSetVertexArray.operator[](i);
            sf::Vertex vertexB = tileSetVertexArray.operator[](i+1);
            sf::Vertex vertexC = tileSetVertexArray.operator[](i+2);
            sf::Vertex vertexD = tileSetVertexArray.operator[](i+3);

            bool isVertexAVisible = isVertexOfQuadVisible(target, vertexA);
            bool isVertexBVisible = isVertexOfQuadVisible(target, vertexB);
            bool isVertexCVisible = isVertexOfQuadVisible(target, vertexC);
            bool isVertexDVisible = isVertexOfQuadVisible(target, vertexD);

            // If any of the vertices of the tile are visible in the view threshold, then just include the whole tile.
            if (isVertexAVisible || isVertexBVisible || isVertexCVisible || isVertexDVisible)
            {
                tileVerticesInViewPerTileset.at(tileSet.path).append(vertexA);
                tileVerticesInViewPerTileset.at(tileSet.path).append(vertexB);
                tileVerticesInViewPerTileset.at(tileSet.path).append(vertexC);
                tileVerticesInViewPerTileset.at(tileSet.path).append(vertexD);
            }
        }
    }

    return tileVerticesInViewPerTileset;
}

bool ViewManager::isVertexOfQuadVisible(const sf::RenderTarget& target, const sf::Vertex& vertex)
{
    sf::Vector2f viewCentre = target.getView().getCenter();

    float minOffsetX = viewCentre.x - (Crucible::TILE_SIZE * TOTAL_TILES_VISIBLE_X);
    minOffsetX = minOffsetX < 0 ? 0.0f : minOffsetX;
    float minOffsetY = viewCentre.y - (Crucible::TILE_SIZE * TOTAL_TILES_VISIBLE_Y);
    minOffsetY = minOffsetY < 0 ? 0.0f : minOffsetY;

    float maxOffsetX = viewCentre.x + (Crucible::TILE_SIZE * TOTAL_TILES_VISIBLE_X);
    float maxOffsetY = viewCentre.y + (Crucible::TILE_SIZE * TOTAL_TILES_VISIBLE_Y);

    //bool vertexInViewOnX = vertex.position.x >= 0 && vertex.position.x < 1000; //vertex.position.x >= minOffsetX && vertex.position.x <= maxOffsetX;
    bool vertexInViewOnX = vertex.position.x >= minOffsetX && vertex.position.x <= maxOffsetX;
    bool vertexInViewOnY = vertex.position.y >= minOffsetY && vertex.position.y <= maxOffsetY;

    return vertexInViewOnX && vertexInViewOnY;
}
