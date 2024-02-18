#include "light_collision_system.h"

LightCollisionSystem::LightCollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LightCollisionSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();

    for (const Entity entity : entities)
    {
        if (!entity.hasComponent<Component::CLightSource>())
        {
            continue;
        }

        resolveLightCollisions(entity);
    }
}

void LightCollisionSystem::resolveLightCollisions(const Entity& entity)
{
    auto& lightSource = entity.getComponent<Component::CLightSource>();
    std::vector<Entity> players = m_entityManager.getEntitiesByEntityType(Crucible::EntityType::PLAYER);

    for (size_t rayIndex = 0; rayIndex < lightSource.rays.size(); rayIndex++)
    {
        Crucible::Ray& ray = lightSource.rays[rayIndex];

        ObjectLayer& layer = LevelManager::activeLevel.layerNameToObjectLayer.at(lightSource.lightingObjectLayerName);
        checkForLightIntersectWithObject(lightSource, layer.lightingObjectData, ray, rayIndex);

        for (Entity& playerEntity : players)
        {
            auto& cTile = playerEntity.getComponent<Component::CTile>();
            std::vector<Object> playerObjects{
                    {Crucible::EntityType::PLAYER, cTile.tile.tileIdx, cTile.tile.vertices}
            };

            checkForLightIntersectWithObject(lightSource, playerObjects, ray, rayIndex);
        }
    }
}

void LightCollisionSystem::checkForLightIntersectWithObject(
        Component::CLightSource& lightSource,
        const std::vector<Object>& lightObjects,
        Crucible::Ray& ray,
        size_t lineIndex)
{
    // @Refactor: Rather than order these in reverse, sort by closest distance to line for a more scalable solution.
    // [0, 1] = top, [1, 2] = right, [2, 3] = bottom, [3, 4] = left
    for (const Object& object : lightObjects)
    {
        const sf::VertexArray& objectVertices = *object.objectVertices;
        for (size_t shapeSideIndex = 0; shapeSideIndex < objectVertices.getVertexCount() - 1; shapeSideIndex++)
        {
            const Crucible::Vec2 rayStartPos{ray.getStartVertex()->x, ray.getStartVertex()->y};
            const Crucible::Vec2 rayEndPos{ray.getEndVertex().x, ray.getEndVertex().y};
            const sf::Vertex& objectStartVert = objectVertices[shapeSideIndex];
            const sf::Vertex& objectEndVert = objectVertices[shapeSideIndex + 1];
            const Crucible::Vec2 shapeLineStartPos{objectStartVert.position.x, objectStartVert.position.y};
            const Crucible::Vec2 shapeLineEndPos{objectEndVert.position.x, objectEndVert.position.y};
            const Crucible::LightRayIntersect lightRayIntersect = isLineIntersecting(rayStartPos, rayEndPos,
                    shapeLineStartPos, shapeLineEndPos, object.entityType);

            if (!lightRayIntersect.hasIntersection)
            {
                continue;
            }

            lightSource.lightRayIntersects[lineIndex].emplace_back(lightRayIntersect);
        }
    }
}

Crucible::LightRayIntersect LightCollisionSystem::isLineIntersecting(Crucible::Vec2 vertexA,
        Crucible::Vec2 vertexB, Crucible::Vec2 vertexC, Crucible::Vec2 vertexD, const Crucible::EntityType entityType)
{
    Crucible::Vec2 r = (vertexB - vertexA);
    Crucible::Vec2 s = (vertexD - vertexC);
    Crucible::Vec2 cma = vertexC - vertexA;

    float rCrossS = crossProduct(r, s);
    float cmaCrossS = crossProduct(cma, s);
    float cmaCrossR = crossProduct(cma, r);

    float t = (cmaCrossS / rCrossS);
    float u = (cmaCrossR / rCrossS);

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        float collisionVertexX = vertexA.x + (t * r.x);
        float collisionVertexY = vertexA.y + (t * r.y);
        return {true, entityType, Crucible::Vec2(collisionVertexX, collisionVertexY)};
    }

    return {false, entityType, Crucible::Vec2(0, 0)};
}

float LightCollisionSystem::crossProduct(Crucible::Vec2 a, Crucible::Vec2 b)
{
    return a.x * b.y - b.x * a.y;
}