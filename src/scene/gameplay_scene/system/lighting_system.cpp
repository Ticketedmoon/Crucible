#include "lighting_system.h"

LightingSystem::LightingSystem(EntityManager& entityManager, EntitySpawner& entitySpawner, sf::Clock& gameClock)
    : m_entityManager(entityManager), m_entitySpawner(entitySpawner), m_gameClock(gameClock)
{

}

void LightingSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntitiesByEntityType(Crucible::EntityType::GUARD);
    for (const Entity entity : entities)
    {
        auto& entityLightSource = entity.getComponent<Component::CLightSource>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();
        auto& entityMagicWielder = entity.getComponent<Component::CMagicCaster>();

        entityLightSource.lightVertices.clear();

        // Combination of CORE rays and ADDITIONAL rays
        std::vector<Crucible::LightRayIntersect> rayIntersections = findAllRayIntersectionPoints(
                Crucible::RayType::CORE, entityLightSource, entityTransform, entityMagicWielder);

        // Add additional Rays
        std::vector<Crucible::LightRayIntersect> additionalRayIntersections = findAllRayIntersectionPoints(
                Crucible::RayType::ADDITIONAL, entityLightSource, entityTransform, entityMagicWielder);
        rayIntersections.insert(rayIntersections.end(), additionalRayIntersections.begin(), additionalRayIntersections.end());

        sortIntersectionsByAngleAscending(entityTransform, rayIntersections);
        // Add to light rendering vertex array
        addVerticesForLightCollisions(entityLightSource, entityTransform, rayIntersections);
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
        Crucible::RayType rayType,
        Component::CLightSource& entityLightSource,
        const Component::CTransform& entityTransform,
        Component::CMagicCaster& entityBow)
{
    std::vector<Crucible::LightRayIntersect> collisionPoints;

    Crucible::LightRayGroup& rayGroup = entityLightSource.lightRayGroups[rayType];
    for (size_t rayIndex = 0; rayIndex <  rayGroup.rays.size(); rayIndex++)
    {
        std::vector<Crucible::LightRayIntersect>& intersectList = rayGroup.rayIntersects[rayIndex];
        if (intersectList.empty())
        {
            continue;
        }

        sortLightIntersectionsByDistanceToEntity(entityTransform, intersectList);

        Crucible::LightRayIntersect intersect = intersectList[0];

        if (intersectList[0].entityType == Crucible::EntityType::PLAYER)
        {
            if (m_gameClock.getElapsedTime().asMilliseconds() > entityBow.timeUntilNextProjectileFireMs)
            {
                // spawn projectile
                m_entitySpawner.createProjectile(*entityTransform.position, intersect.collisionPoint);
                // wait 3 seconds
                entityBow.timeUntilNextProjectileFireMs = m_gameClock.getElapsedTime().asMilliseconds() + sf::Int32(3000);
            }

            // Add intersect that is not player
            intersect = *std::ranges::find_if(intersectList,
                    [](const auto& intersect) {
                        return intersect.entityType != Crucible::EntityType::PLAYER;
                    });
        }

        collisionPoints.emplace_back(intersect);

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

void LightingSystem::sortLightIntersectionsByDistanceToEntity(
        const Component::CTransform& entityTransform,
        std::vector<Crucible::LightRayIntersect>& intersectList)
{
    auto comparator =
            [&entityTransform](const Crucible::LightRayIntersect& intersectA, const Crucible::LightRayIntersect& intersectB) {
                double playerDistToIntersectA = entityTransform.position->dist(intersectA.collisionPoint);
                double playerDistToIntersectB = entityTransform.position->dist(intersectB.collisionPoint);
                return playerDistToIntersectA < playerDistToIntersectB;
            };

    std::sort(intersectList.begin(), intersectList.end(), comparator);
}