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
                !entity.hasComponent<Component::CTile>())
        {
            continue;
        }

        auto& entityLightSource = entity.getComponent<Component::CLightSource>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();

        entityLightSource.lightVertices.clear();

        std::vector<Crucible::LightRayIntersect> intersections = findAllRayIntersectionPoints(entityLightSource,
                entityTransform);

        sortIntersectionsByAngleAscending(entityTransform, intersections);

        // Add to light rendering vertex array
        addVerticesForLightCollisions(entityLightSource, entityTransform, intersections);
    }
}

void LightingSystem::sortIntersectionsByAngleAscending(
        const Component::CTransform& entityTransform,
        std::vector<Crucible::LightRayIntersect>& intersections)
{
    auto comparator =
            [&entityTransform](const Crucible::LightRayIntersect& intersectA, const Crucible::LightRayIntersect& intersectB) {
                const Crucible::Vec2 velocityA{intersectA.collisionPoint.x - entityTransform.position->x,
                                              intersectA.collisionPoint.y - entityTransform.position->y};
                const Crucible::Vec2 velocityB{intersectB.collisionPoint.x - entityTransform.position->x,
                                               intersectB.collisionPoint.y - entityTransform.position->y};
                const float angleA = std::atan2(velocityA.y, velocityA.x);
                const float angleB = std::atan2(velocityB.y, velocityB.x);

                return angleA < angleB;
            };

    std::sort(intersections.begin(), intersections.end(), comparator);
}

std::vector<Crucible::LightRayIntersect> LightingSystem::findAllRayIntersectionPoints(
        Component::CLightSource& entityLightSource,
        const Component::CTransform& entityTransform)
{
    std::vector<Crucible::LightRayIntersect> collisionPoints;
    for (size_t lineIndex = 0; lineIndex <  entityLightSource.rays.size(); lineIndex++)
    {
        std::vector<Crucible::LightRayIntersect>& intersectList = entityLightSource.lightRayIntersects[lineIndex];
        if (intersectList.empty())
        {
            continue;
        }

        Crucible::LightRayIntersect closestIntersect = findClosestIntersectForLine(entityTransform, intersectList);
        collisionPoints.emplace_back(closestIntersect);

        intersectList.clear();
    }
    return collisionPoints;
}

void LightingSystem::addVerticesForLightCollisions(
        Component::CLightSource& entityLightSource,
        const Component::CTransform& entityTransform,
        const std::vector<Crucible::LightRayIntersect>& intersections)
{
    if (intersections.empty())
    {
        return;
    }

    // Add transform position of player entity.
    entityLightSource.lightVertices.append({{entityTransform.position->x, entityTransform.position->y}, LIGHTING_COLOR});

    // Add all collision points with
    for (const Crucible::LightRayIntersect& intersection : intersections)
    {
        entityLightSource.lightVertices.append({{intersection.collisionPoint.x, intersection.collisionPoint.y},
                                                LIGHTING_COLOR});
    }

    // Add initial collision point again so the TriangleFan can connect forming a full-looking geometric shape displaying visibility.
    entityLightSource.lightVertices.append({{intersections[0].collisionPoint.x, intersections[0].collisionPoint.y},
                                            LIGHTING_COLOR});
}

Crucible::LightRayIntersect LightingSystem::findClosestIntersectForLine(
        const Component::CTransform& entityTransform,
        std::vector<Crucible::LightRayIntersect>& intersectList)
{
    Crucible::LightRayIntersect closestIntersect = intersectList[0];
    double distToPlayer = entityTransform.position->dist(intersectList[0].collisionPoint);
    for (Crucible::LightRayIntersect intersect: intersectList)
    {
        double nextDistToPlayer = entityTransform.position->dist(intersect.collisionPoint);
        if (nextDistToPlayer < distToPlayer)
        {
            distToPlayer = nextDistToPlayer;
            closestIntersect = intersect;
        }
    }
    return closestIntersect;
}