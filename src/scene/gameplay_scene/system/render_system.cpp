#include "render_system.h"

RenderSystem::RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
    configureTextRendering();
}

void RenderSystem::execute()
{
    drawEntities();
    drawGuiData();
}

void RenderSystem::drawEntities()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity e : entities)
    {
        if (e.hasComponent<Component::CRectangleShape>())
        {
            auto& cRectangleShape = e.getComponent<Component::CRectangleShape>();
            m_renderTarget.draw(cRectangleShape.shape);
        }
    }
}

void RenderSystem::drawGuiData()
{

}

// TODO move to engine for inter-scene text drawing?
void RenderSystem::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);

    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.6f);
    text.setLetterSpacing(3.0f);

    m_renderTarget.draw(text);
}

void RenderSystem::configureTextRendering()
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    m_ammoText = sf::Text("", m_font);
}