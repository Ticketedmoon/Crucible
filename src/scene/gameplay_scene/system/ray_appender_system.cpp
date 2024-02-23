#include "ray_appender_system.h"

RayAppenderSystem::RayAppenderSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void RayAppenderSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (auto& entity: entities)
    {
        if (!entity.hasComponent<Component::CLightSource>())
        {
            continue;
        }

        auto& entityLightSource = entity.getComponent<Component::CLightSource>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();
        ObjectLayer lightingLayerForLightSrc
            = LevelManager::activeLevel.layerNameToObjectLayer.at(entityLightSource.lightingObjectLayerName);

        int otherShapeIndex = 0;
        for (Object& object : lightingLayerForLightSrc.lightingObjectData)
        {
            sf::VertexArray objectVertices = *object.objectVertices;

            for (size_t vertIndex = 0; vertIndex < objectVertices.getVertexCount(); vertIndex += totalRaysPerVertex)
            {
                const sf::Vertex& v = objectVertices[vertIndex / totalRaysPerVertex];

                // Update ray vector to point left and right of endV with large RAY_SCALE.
                const size_t additionalRayIndex = TOTAL_CORE_RAYS + vertIndex + (totalRaysPerVertex * otherShapeIndex);

                const Crucible::Vec2& rayEndPositionLeftOfVertex = Crucible::Vec2(v.position.x - RAY_DELTA,
                        v.position.y);
                const Crucible::Vec2& rayEndPositionRightOfVertex = Crucible::Vec2(v.position.x + RAY_DELTA,
                        v.position.y);

                // Scale rays
                Crucible::Vec2 playerPosition{entityTransform.position->x, entityTransform.position->y};
                Crucible::Vec2 endVa = ((rayEndPositionLeftOfVertex - playerPosition) * RAY_SCALE) + playerPosition;
                Crucible::Vec2 endVb = ((rayEndPositionRightOfVertex - playerPosition) * RAY_SCALE) + playerPosition;

                const float angleARads = std::atan2(endVa.y, endVa.x) + RADIANS_OFFSET;
                const float angleBRads = std::atan2(endVb.y, endVb.x) - RADIANS_OFFSET;

                Crucible::Vec2 endVResultA(endVa.x + std::cos(angleARads), endVa.y + std::sin(angleARads));
                Crucible::Vec2 endVResultB(endVb.x + std::cos(angleBRads), endVb.y + std::sin(angleBRads));

                entityLightSource.rays[additionalRayIndex] = Crucible::Ray(entityTransform.position, endVResultA);
                entityLightSource.rays[additionalRayIndex + 1] = Crucible::Ray(entityTransform.position, endVResultB);
            }

            otherShapeIndex++;
        }
    }
}
