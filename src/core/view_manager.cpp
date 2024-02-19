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