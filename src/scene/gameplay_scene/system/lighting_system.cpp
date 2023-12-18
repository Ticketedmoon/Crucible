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
    //entityLightSource.lightVertices.append({{entityTransform.position.x, entityTransform.position.y}, sf::Color::Yellow});

    for (int lineIndex = 0; lineIndex < totalLines-1; lineIndex++)
    {
        std::vector<Crucible::LightRayIntersect>& intersectList = entityLightSource.lightRayIntersects[lineIndex];
        if (intersectList.empty())
        {
            continue;
        }

        // Find closest intersect point.
        Crucible::LightRayIntersect closestIntersect = findClosestIntersectForLine(entityTransform, intersectList);

        entityLightSource.lightVertices.append({{entityTransform.position.x, entityTransform.position.y}, sf::Color::Yellow});
        addVertexForTriangleFanLightSource(entityLightSource, closestIntersect);

        // Clear intersects after finding closest intersect.
        intersectList.clear();
    }
}

// If B is colliding with any shape, take the nearest line segment corner to the previous rays end point.
// Q: Do we base it off the previous rays collision point? Or corner point
void LightingSystem::addVertexForTriangleFanLightSource(Component::CLightSource& entityLightSource,
        const Crucible::LightRayIntersect& closestIntersect) const
{
    if (closestIntersect.isShapeIntersection)
    {
        double distToStartVertex = closestIntersect.collisionPoint.dist(closestIntersect.collidedLineStartVertex);
        double distToEndVertex = closestIntersect.collisionPoint.dist(closestIntersect.collidedLineEndVertex);
        if (distToStartVertex < distToEndVertex)
        {
            entityLightSource.lightVertices.append({{closestIntersect.collidedLineStartVertex.x, closestIntersect.collidedLineStartVertex.y}, sf::Color::Yellow});
        }
        else
        {
            entityLightSource.lightVertices.append({{closestIntersect.collidedLineEndVertex.x, closestIntersect.collidedLineEndVertex.y}, sf::Color::Yellow});
        }
    }
    else
    {
        entityLightSource.lightVertices.append({{closestIntersect.collisionPoint.x, closestIntersect.collisionPoint.y}, sf::Color::Yellow});
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
