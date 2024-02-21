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
        static std::unordered_map<std::string, sf::VertexArray> getTileVerticesInView(
                const sf::RenderTarget& target,
                const TileLayer& tileLayer,
                const std::vector<TileSet>& tileSets);

    private:
        ViewManager();
        static float getViewCentreForCoordinate(float playerCoordinatePosition, float levelDimension,
                                                float windowDimensionValue, float playerDimensionValue);
        static bool isVertexOfQuadVisible(const sf::RenderTarget& target, const sf::Vertex& vertex);

    private:
        static const uint32_t TOTAL_TILES_VISIBLE_X = std::ceil(Crucible::WINDOW_WIDTH / Crucible::TILE_SIZE / 6);
        static const uint32_t TOTAL_TILES_VISIBLE_Y = std::ceil(Crucible::WINDOW_HEIGHT / Crucible::TILE_SIZE / 6);
};

#endif //CRUCIBLE_VIEW_MANAGER_H