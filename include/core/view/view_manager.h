#pragma once

#ifndef CRUCIBLE_VIEW_MANAGER_H
#define CRUCIBLE_VIEW_MANAGER_H

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>

#include "core/engine/common_constants.h"
#include "core/manager/level_manager.h"

#include "entity.h"

class ViewManager
{
    public:
        static sf::View centerViewOnEntity(sf::RenderTarget& renderTarget, const Entity& entity, float zoomFactor);
        static std::unordered_map<std::string, sf::VertexArray> getTileVerticesInView(
                const sf::RenderTarget& target,
                const TileLayer& tileLayer,
                const std::vector<TileSet>& tileSets);

    private:
        ViewManager();
        static float getViewCentreForCoordinate(float playerCoordinatePosition, float levelDimension,
                                                float windowDimensionValue, float playerDimensionValue);
        static bool isVertexOfQuadVisible(sf::Vector2f vertexPosition, sf::FloatRect screenRect);
};

#endif //CRUCIBLE_VIEW_MANAGER_H