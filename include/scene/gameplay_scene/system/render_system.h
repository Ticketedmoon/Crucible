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
        explicit GameplayRenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager);

        void execute() override;

    private:
        void drawEntities();

        void drawGuiData();
        void configureTextRendering();
        void drawText(sf::Text& text, const sf::Color& fillColour, uint8_t characterSize, sf::Vector2f position);

    private:
        sf::RenderTarget& m_renderTarget;
        EntityManager& m_entityManager;
        static inline sf::Font m_font;
};


#endif //CRUCIBLE_GAMEPLAY_RENDER_SYSTEM_H
