#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    Vec2 position{Crucible::WINDOW_WIDTH / 2.0f, Crucible::WINDOW_HEIGHT - 128};
    Vec2 dimensions{50, 50};

    std::vector<Crucible::Vertex> rayStartVertices;
    std::vector<Crucible::Vertex> rayEndVertices;

    constexpr uint8_t rayGrowSpeed = 10;

    // FIXME, can we reuse the same ray start vertex? It will always be the same originating point.
    rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));
    rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));
    rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));
    rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));
    rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));
    rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));
    rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));
    rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));

    // TODO put ray coords into separate file

    // Up
    rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, -rayGrowSpeed}, sf::Color::Yellow));
    // Down
    rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, rayGrowSpeed}, sf::Color::Yellow));
    // Left
    rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {-rayGrowSpeed, 0}, sf::Color::Yellow));
    // Right
    rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {rayGrowSpeed, 0}, sf::Color::Yellow));

    // Diagonal-Up-Left
    rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {-rayGrowSpeed, -rayGrowSpeed}, sf::Color::Yellow));
    // Diagonal-Up-Right
    rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {rayGrowSpeed, -rayGrowSpeed}, sf::Color::Yellow));
    // Diagonal-Down-Left
    rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {-rayGrowSpeed, rayGrowSpeed}, sf::Color::Yellow));
    // Diagonal-Down-Right
    rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {rayGrowSpeed, rayGrowSpeed}, sf::Color::Yellow));

    std::cout << "Found: [" << rayEndVertices.size() << "] light rays" << '\n';

    sf::VertexArray shapeVertices(sf::Quads);
    shapeVertices.append(sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));
    shapeVertices.append(sf::Vertex({position.x + dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));
    shapeVertices.append(sf::Vertex({position.x + dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Green));
    shapeVertices.append(sf::Vertex({position.x - dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Green));
    shapeVertices.append(sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));

    // FIXME temp magic num
    std::vector<std::vector<Crucible::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Crucible::LightRayIntersect>>(8, std::vector<Crucible::LightRayIntersect>());

    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(shapeVertices);
    e.addComponent<Component::CLightSource>(rayStartVertices, rayEndVertices, sf::VertexArray(), defaultLightRayIntersects);
}

void EntitySpawner::spawnWall(Vec2 position, Vec2 dimensions)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::WALL);

    sf::VertexArray vertices(sf::Quads, 5);
    vertices[0] = sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Blue);
    vertices[1] = sf::Vertex({position.x + dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Blue);
    vertices[2] = sf::Vertex({position.x + dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Blue);
    vertices[3] = sf::Vertex({position.x - dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Blue);
    vertices[4] = sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Blue);

    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(vertices);
}