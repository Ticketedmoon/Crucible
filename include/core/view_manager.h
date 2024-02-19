#pragma once

#ifndef CRUCIBLE_VIEW_MANAGER_H
#define CRUCIBLE_VIEW_MANAGER_H

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>

#include "common_constants.h"
#include "level_manager.h"
#include "entity.h"

static const float VIEW_ZOOM_FACTOR = 0.5;

class ViewManager
{
    public:
        static sf::View centerViewOnEntity(sf::RenderTarget& renderTarget, const Entity& entity);

    private:
        ViewManager();
        static float getViewCentreForCoordinate(float playerCoordinatePosition, float levelDimension,
                                                float windowDimensionValue, float playerDimensionValue);
};

#endif //CRUCIBLE_VIEW_MANAGER_H