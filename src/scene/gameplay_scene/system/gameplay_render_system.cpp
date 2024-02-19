#include "render_system.h"
#include "view_manager.h"

GameplayRenderSystem::GameplayRenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager,
        TextureManager& textureManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager), m_textureManager(textureManager)
{
    configureTextRendering();
}

void GameplayRenderSystem::execute()
{
    centreViewOnPlayer();
    drawMap();
    drawEntities();
    drawGuiData();
}

void GameplayRenderSystem::centreViewOnPlayer()
{
    std::vector<Entity>& players = m_entityManager.getEntitiesByEntityType(Crucible::EntityType::PLAYER);
    if (!players.empty())
    {
        // Reset the view
        m_renderTarget.setView(m_renderTarget.getDefaultView());

        // Set to Player Centre
        ViewManager::centerViewOnEntity(m_renderTarget, players.at(0));
    }
}

void GameplayRenderSystem::drawMap()
{
    for (const TileLayer& layer : LevelManager::activeLevel.tileLayers)
    {
        const std::unordered_map<std::string, sf::VertexArray>& verticesInViewPerTileset
            = ViewManager::getTileVerticesInView(m_renderTarget, layer, LevelManager::activeLevel.tileSets);
        for (const TileSet& tileSet: LevelManager::activeLevel.tileSets)
        {
            const sf::RenderStates& renderStates = sf::RenderStates(m_textureManager.getTexture(tileSet.path).get());
            m_renderTarget.draw(verticesInViewPerTileset.at(tileSet.path), renderStates);
        }
    }
}

void GameplayRenderSystem::drawEntities()
{
    std::vector<Entity> entities = m_entityManager.getEntities();

    const sf::RenderStates lightSrcRenderStates{sf::BlendMultiply};

    for (const Entity e : entities)
    {
        if (e.hasComponent<Component::CLightSource>())
        {
            auto& cLightSource = e.getComponent<Component::CLightSource>();
            sf::VertexArray& lightVertices = cLightSource.lightVertices;
            m_renderTarget.draw(&lightVertices[0], lightVertices.getVertexCount(), sf::TriangleFan, lightSrcRenderStates);
        }

        if (e.hasComponent<Component::CTile>())
        {
            auto& cTile = e.getComponent<Component::CTile>();
            sf::RenderStates renderStates{cTile.texture.get()};
            m_renderTarget.draw(*cTile.tile.vertices, renderStates);
        }
    }
}

void GameplayRenderSystem::drawGuiData()
{

}

// TODO move to engine for inter-scene text drawing?
void GameplayRenderSystem::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);

    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.6f);
    text.setLetterSpacing(3.0f);

    m_renderTarget.draw(text);
}

void GameplayRenderSystem::configureTextRendering()
{
    // NOT IMPLEMENTED
}