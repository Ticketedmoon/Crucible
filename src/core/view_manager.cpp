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
        tileVerticesInViewPerTileset.insert({tileSet.path, sf::VertexArray(sf::Quads)});
    }

    sf::View cam = target.getView();
    sf::FloatRect screenRect(cam.getCenter() - (cam.getSize() / 2.f), cam.getSize());

    for (const TileSet& tileSet : tileSets)
    {
        const sf::VertexArray& tileSetVertexArray = tileLayer.tilesetPathToLevelData.at(tileSet.path);
        for (size_t i = 0; i < tileSetVertexArray.getVertexCount(); i+=4)
        {
            const sf::Vertex vertexA = tileSetVertexArray.operator[](i);
            const sf::Vertex vertexB = tileSetVertexArray.operator[](i+1);
            const sf::Vertex vertexC = tileSetVertexArray.operator[](i+2);
            const sf::Vertex vertexD = tileSetVertexArray.operator[](i+3);

            bool isVertexAVisible = isVertexOfQuadVisible(vertexA.position, screenRect);
            bool isVertexBVisible = isVertexOfQuadVisible(vertexB.position, screenRect);
            bool isVertexCVisible = isVertexOfQuadVisible(vertexC.position, screenRect);
            bool isVertexDVisible = isVertexOfQuadVisible(vertexD.position, screenRect);

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

bool ViewManager::isVertexOfQuadVisible(const sf::Vector2f vertexPosition, sf::FloatRect screenRect)
{
    bool vertexInViewOnX = vertexPosition.x >= screenRect.left && vertexPosition.x <= screenRect.left + screenRect.width;
    bool vertexInViewOnY = vertexPosition.y >= screenRect.top && vertexPosition.y <= screenRect.top + screenRect.height;
    return vertexInViewOnX && vertexInViewOnY;
}
