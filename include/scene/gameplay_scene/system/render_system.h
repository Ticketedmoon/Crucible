#pragma once

#ifndef CRUCIBLE_GAMEPLAY_RENDER_SYSTEM_H
#define CRUCIBLE_GAMEPLAY_RENDER_SYSTEM_H

#include <cmath>
#include <cassert>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "system.h"
#include "entity/entity_manager.h"
#include "scene/gameplay_scene/system/render_system.h"
#include "core/manager/level_manager.h"
#include "core/view/view_manager.h"

class GameplayRenderSystem : public System
{
    public:
        explicit GameplayRenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager,
                TextureManager& textureManager);

        void execute() override;

    private:
        void centreViewOnPlayer();
        void configureOverlays();

        void draw();
        void drawMap();
        void drawEntities();
        void drawGuiData();
        void drawTiles(const std::unordered_map<std::string, sf::VertexArray>& tileData);
        void drawOverlays();

    private:
        static inline uint16_t PLAYER_LIGHT_OVERLAY_RADIUS = 100;
        static inline sf::Font m_font;

        sf::RectangleShape m_darkOverlay;
        sf::CircleShape m_playerLightOverlay;

        sf::RenderTarget& m_renderTarget;
        EntityManager& m_entityManager;
        TextureManager& m_textureManager;
};


#endif //CRUCIBLE_GAMEPLAY_RENDER_SYSTEM_H
