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
#include "level_manager.h"

class GameplayRenderSystem : public System
{
    public:
        explicit GameplayRenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager,
                TextureManager& textureManager);

        void execute() override;

    private:
        void centreViewOnPlayer();
        void drawMap();
        void drawEntities();
        void drawGuiData();
        void drawTiles(const std::unordered_map<std::string, sf::VertexArray>& tileData);

        void configureTextRendering();
        void drawText(sf::Text& text, const sf::Color& fillColour, uint8_t characterSize, sf::Vector2f position);

    private:
        static inline sf::Font m_font;

        sf::RenderTarget& m_renderTarget;
        EntityManager& m_entityManager;
        TextureManager& m_textureManager;
};


#endif //CRUCIBLE_GAMEPLAY_RENDER_SYSTEM_H
