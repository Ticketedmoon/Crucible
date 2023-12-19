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
    size_t totalLines = entityLightSource.rays.size();

    // Add player position as starting vertex
    //entityLightSource.lightVertices.append({{m_entityTransform.position.x, m_entityTransform.position.y}, sf::Color::Yellow});

    for (size_t lineIndex = 0; lineIndex < totalLines-1; lineIndex++)
    {
        std::vector<Crucible::LightRayIntersect>& intersectList = entityLightSource.lightRayIntersects[lineIndex];
        if (intersectList.empty())
        {
            continue;
        }

        // Find closest intersect point.
        Crucible::LightRayIntersect closestIntersect = findClosestIntersectForLine(entityTransform, intersectList);

        entityLightSource.lightVertices.append({{entityTransform.position.x, entityTransform.position.y}, sf::Color::Yellow});
        entityLightSource.lightVertices.append({{closestIntersect.collisionPoint.x, closestIntersect.collisionPoint.y}, sf::Color::Yellow});

        // Clear intersects after finding closest intersect.
        intersectList.clear();
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
