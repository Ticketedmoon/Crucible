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
#include "scene.h"
#include "victory_scene.h"

#include "entity/entity_manager.h"
#include "scene/gameplay_scene/system/gameplay_render_system.h"
#include "core/manager/level_manager.h"
#include "core/view/view_manager.h"
#include "common_properties.h"

class GameplayRenderSystem : public System
{
    public:
        explicit GameplayRenderSystem(
                GameEngine& m_gameEngine,
                EntityManager& entityManager,
                TextureManager& textureManager,
                Crucible::GameProperties& gameProperties);

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

        static void lowerOverlayOpacity(sf::Shape& overlayShape);
        void updateOverlayForLevelCompletion();

    private:
        static inline constexpr uint16_t PLAYER_LIGHT_OVERLAY_RADIUS = 100;
        static inline sf::Font m_font;

        sf::RectangleShape m_darkOverlay;
        sf::CircleShape m_playerLightOverlay;

        GameEngine& m_gameEngine;
        EntityManager& m_entityManager;
        TextureManager& m_textureManager;
        Crucible::GameProperties& m_gameProperties;
};


#endif //CRUCIBLE_GAMEPLAY_RENDER_SYSTEM_H
