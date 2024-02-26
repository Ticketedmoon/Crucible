#include "render_system.h"

GameplayRenderSystem::GameplayRenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager,
        TextureManager& textureManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager), m_textureManager(textureManager)
{
    configureOverlays();
}

void GameplayRenderSystem::execute()
{
    centreViewOnPlayer();
    draw();
}

void GameplayRenderSystem::draw()
{
    drawMap();
    drawEntities();
    drawGuiData();
    drawOverlays();
}

void GameplayRenderSystem::centreViewOnPlayer()
{
    std::vector<Entity>& players = m_entityManager.getEntitiesByEntityType(Crucible::EntityType::PLAYER);
    if (!players.empty())
    {
        // Reset the view
        m_renderTarget.setView(m_renderTarget.getDefaultView());
        // Set to Player Centre
        ViewManager::centerViewOnEntity(m_renderTarget, players.at(0), Crucible::PLAYER_ZOOM_FACTOR);
    }
}

void GameplayRenderSystem::drawMap()
{
    for (const TileLayer& layer: LevelManager::activeLevel.tileLayers)
    {
        if (Crucible::SHOULD_CULL_TILES)
        {
            const std::unordered_map<std::string, sf::VertexArray>& tileVerticesInView
                    = ViewManager::getTileVerticesInView(m_renderTarget, layer, LevelManager::activeLevel.tileSets);
            drawTiles(tileVerticesInView);
        }
        else
        {
            drawTiles(layer.tilesetPathToLevelData);
        }
    }
}

void GameplayRenderSystem::drawTiles(const std::unordered_map<std::string, sf::VertexArray>& tileData)
{
    for (const TileSet& tileSet: LevelManager::activeLevel.tileSets)
    {
        const sf::RenderStates& renderStates = sf::RenderStates(m_textureManager.getTexture(tileSet.path).get());
        m_renderTarget.draw(tileData.at(tileSet.path), renderStates);
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

void GameplayRenderSystem::drawOverlays()
{
    m_renderTarget.draw(m_darkOverlay, sf::BlendMultiply);
    std::vector<Entity>& players = m_entityManager.getEntitiesByEntityType(Crucible::EntityType::PLAYER);
    if (players.empty())
    {
        return;
    }

    auto& transform = players.at(0).getComponent<Component::CTransform>();
    m_playerLightOverlay.setPosition(
            transform.position->x + transform.dimensions.x/2 - PLAYER_LIGHT_OVERLAY_RADIUS,
            transform.position->y + transform.dimensions.y/2 - PLAYER_LIGHT_OVERLAY_RADIUS);
    m_renderTarget.draw(m_playerLightOverlay, sf::BlendAdd);
}

void GameplayRenderSystem::drawGuiData()
{
    // NOT IMPLEMENTED
}

void GameplayRenderSystem::configureOverlays()
{
    m_darkOverlay.setPosition(sf::Vector2f(0, 0));
    m_darkOverlay.setFillColor(sf::Color(255, 255, 255, 64));
    m_darkOverlay.setSize(sf::Vector2f(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));

    m_playerLightOverlay.setFillColor(sf::Color(0, 0, 0, 64));
    m_playerLightOverlay.setRadius(PLAYER_LIGHT_OVERLAY_RADIUS);
}