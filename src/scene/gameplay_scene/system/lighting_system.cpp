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

        addVerticesForLightCollisions(entityLightSource, entityTransform);
    }
}

void LightingSystem::addVerticesForLightCollisions(Component::CLightSource& entityLightSource,
        const Component::CTransform& entityTransform) const
{
    size_t totalLines = entityLightSource.rayStartVertices.size();

    for (int lineIndex = 0; lineIndex < totalLines; lineIndex += 2)
    {
        std::vector<Crucible::LightRayIntersect>& intersectListA = entityLightSource.lightRayIntersects[lineIndex];
        std::vector<Crucible::LightRayIntersect>& intersectListB = entityLightSource.lightRayIntersects[lineIndex+1];
        if (intersectListA.empty() || intersectListB.empty())
        {
            continue;
        }

        // Find closest intersect point.
        Crucible::LightRayIntersect closestIntersectA = findClosestIntersectForLine(entityTransform, intersectListA);
        Crucible::LightRayIntersect closestIntersectB = findClosestIntersectForLine(entityTransform, intersectListB);

        // Clear intersects after finding closest intersect.
        intersectListA.clear();
        intersectListB.clear();

        // Add player position as starting vertex
        entityLightSource.lightVertices.append({{entityTransform.position.x, entityTransform.position.y}, sf::Color::Yellow});
        entityLightSource.lightVertices.append({{closestIntersectA.collisionPoint.x, closestIntersectA.collisionPoint.y}, sf::Color::Yellow});
        entityLightSource.lightVertices.append({{entityTransform.position.x, entityTransform.position.y}, sf::Color::Yellow});
        entityLightSource.lightVertices.append({{closestIntersectB.collisionPoint.x, closestIntersectB.collisionPoint.y}, sf::Color::Yellow});
    }
}

Crucible::LightRayIntersect LightingSystem::findClosestIntersectForLine(const Component::CTransform& entityTransform,
        std::vector<Crucible::LightRayIntersect>& intersectList) const
{
    Crucible::LightRayIntersect closestIntersect = intersectList[0];
    double distToPlayer = entityTransform.position.dist(intersectList[0].collisionPoint);
    for (Crucible::LightRayIntersect intersect: intersectList)
    {
        double nextDistToPlayer = entityTransform.position.dist(intersect.collisionPoint);
        if (nextDistToPlayer < distToPlayer)
        {
            distToPlayer = nextDistToPlayer;
            closestIntersect = intersect;
        }
    }
    return closestIntersect;
}
