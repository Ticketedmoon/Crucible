#pragma once

#ifndef CRUCIBLE_VICTORY_RENDER_SYSTEM_H
#define CRUCIBLE_VICTORY_RENDER_SYSTEM_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "system.h"
#include "entity_manager.h"

class VictoryRenderSystem : public System
{
    public:
        explicit VictoryRenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager);

        void execute() override;

    private:
        void drawText(sf::Text& text, const sf::Color& fillColour, uint8_t characterSize, sf::Vector2f position);

        static void configureTextRendering();

    private:
        static const inline std::string FONT_PATH = "resources/fonts/breathe_fire.ttf";
        static constexpr int TEXT_POSITION_OFFSET = 64;
        static inline sf::Font m_font;
        static inline sf::Text m_text;

        sf::RenderTarget& m_renderTarget;
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_VICTORY_RENDER_SYSTEM_H
