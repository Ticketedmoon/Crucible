#include "victory_scene/system/victory_render_system.h"

VictoryRenderSystem::VictoryRenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
    configureTextRendering();
}

void VictoryRenderSystem::execute()
{
    m_renderTarget.setView(m_renderTarget.getDefaultView());

    const sf::Vector2u& windowSize = m_renderTarget.getSize();
    float viewCentreX = m_renderTarget.getView().getCenter().x;
    const sf::Vector2<float>& position = sf::Vector2f(
            viewCentreX - windowSize.x/5,
            windowSize.y / 3 - TEXT_POSITION_OFFSET);
    drawText(m_text, sf::Color::Green, 60, position);
}

// TODO move to engine for inter-scene text drawing?
void VictoryRenderSystem::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(0.6f);
    text.setLetterSpacing(5.0f);

    m_renderTarget.draw(text);
}

void VictoryRenderSystem::configureTextRendering()
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    m_text = sf::Text("Level 1 Complete\n\nWell Done!\n\nContinue?", m_font);
}