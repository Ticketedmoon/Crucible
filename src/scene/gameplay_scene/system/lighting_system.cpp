#include "lighting_system.h"

LightingSystem::LightingSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LightingSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity entity : entities)
    {
        if (!entity.hasComponent<Component::CLightSource>() || !entity.hasComponent<Component::CTransform>())
        {
            continue;
        }

        auto& entityLightSource = entity.getComponent<Component::CLightSource>();

        // Clear current light vertices
        entityLightSource.lightVertices.clear();

        if (entityLightSource.lightRayIntersects.empty())
        {
            continue;
        }

        // Sort intersect points based on closed to player (use Vec2 Distance formula)
        Component::CTransform cTransform = entity.getComponent<Component::CTransform>();

        // Note: This will only work for 1 hit, if we want multiple ray hits we'll need a more scalable solution.
        //       Add this later.
        Crucible::LightRayIntersect closestIntersect = entityLightSource.lightRayIntersects.at(0);
        double distToPlayer = cTransform.position.dist(entityLightSource.lightRayIntersects.at(0).pos);
        for (Crucible::LightRayIntersect intersect : entityLightSource.lightRayIntersects)
        {
            double nextDistToPlayer = cTransform.position.dist(intersect.pos);
            if (nextDistToPlayer < distToPlayer)
            {
                distToPlayer = nextDistToPlayer;
                closestIntersect = intersect;
            }
        }

        // Clear intersects after finding closest intersect.
        entityLightSource.lightRayIntersects.clear();

        // Add that item to lightVertices array.
        Component::CShape entityRectangleShape = entity.getComponent<Component::CShape>();
        float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

        entityLightSource.lightVertices.append(sf::Vertex({cTransform.position.x, cTransform.position.y - yDiff}, sf::Color::Yellow));
        entityLightSource.lightVertices.append(sf::Vertex({closestIntersect.pos.x, closestIntersect.pos.y}, sf::Color::Yellow));
    }
}
