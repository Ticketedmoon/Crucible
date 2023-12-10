#include "lighting_system.h"

LightingSystem::LightingSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LightingSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity entity : entities)
    {
        if (!entity.hasComponent<Component::CLightSource>() || !entity.hasComponent<Component::CTransform>() ||
                !entity.hasComponent<Component::CShape>())
        {
            continue;
        }

        auto& entityLightSource = entity.getComponent<Component::CLightSource>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();

        // Clear current light vertices
        entityLightSource.lightVertices.clear();

        size_t totalLines = entityLightSource.rayStartVertices.size();
        for (int lineIndex = 0; lineIndex < totalLines; lineIndex++)
        {

            if (entityLightSource.lightRayIntersects[lineIndex].empty())
            {
                continue;
            }

            // Find closest intersect point.
            // Note: This will only work for 1 hit, if we want multiple ray hits we'll need a more scalable solution around
            //       sorting nearest objects and such - add this later.
            Crucible::LightRayIntersect closestIntersect = entityLightSource.lightRayIntersects[lineIndex][0];
            double distToPlayer = entityTransform.position.dist(entityLightSource.lightRayIntersects[lineIndex][0].collisionPoint);
            for (Crucible::LightRayIntersect intersect: entityLightSource.lightRayIntersects[lineIndex])
            {
                double nextDistToPlayer = entityTransform.position.dist(intersect.collisionPoint);
                if (nextDistToPlayer < distToPlayer)
                {
                    distToPlayer = nextDistToPlayer;
                    closestIntersect = intersect;
                }
            }

            // Clear intersects after finding closest intersect.
            entityLightSource.lightRayIntersects[lineIndex].clear();

            // Add that item to lightVertices array.
            entityLightSource.lightVertices.append({{entityTransform.position.x, entityTransform.position.y}, sf::Color::Yellow});
            entityLightSource.lightVertices.append({{closestIntersect.collisionPoint.x, closestIntersect.collisionPoint.y}, sf::Color::Yellow});

            // debug
            //entityLightSource.lightVertices.append(sf::Vertex({closestIntersect.nearestShapeVertices[0].x, closestIntersect.nearestShapeVertices[0].y}, sf::Color::Yellow));
            //entityLightSource.lightVertices.append(sf::Vertex({closestIntersect.nearestShapeVertices[1].x, closestIntersect.nearestShapeVertices[1].y}, sf::Color::Yellow));
        }
    }
}
