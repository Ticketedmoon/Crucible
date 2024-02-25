#include "ray_appender_system.h"

RayAppenderSystem::RayAppenderSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void RayAppenderSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntitiesByEntityType(Crucible::EntityType::GUARD);
    for (auto& entity: entities)
    {
        auto& entityLightSource = entity.getComponent<Component::CLightSource>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();

        ObjectLayer lightingLayerForLightSrc
            = LevelManager::activeLevel.layerNameToObjectLayer.at(entityLightSource.lightingObjectLayerName);

        Crucible::LightRayGroup& rayGroup = entityLightSource.lightRayGroups.at(Crucible::RayType::ADDITIONAL);

        size_t additionalRayGroupStartIdx = 0;

        for (const auto& object : lightingLayerForLightSrc.lightingObjectData)
        {
            sf::VertexArray objectVertices = *object.objectVertices;
            const size_t TOTAL_ADDITIONAL_RAYS_FOR_OBJECT = (objectVertices.getVertexCount()-1) * TOTAL_ADDITIONAL_RAYS_PER_VERT;

            for (size_t objectIndex = 0; objectIndex <= TOTAL_ADDITIONAL_RAYS_FOR_OBJECT; objectIndex += TOTAL_ADDITIONAL_RAYS_PER_VERT)
            {
                const sf::Vertex& rayEndVert = objectVertices[objectIndex + 1];
                const Crucible::Vec2& rayEndPositionLeftOfVertex = Crucible::Vec2(rayEndVert.position.x - RAY_DELTA,
                        rayEndVert.position.y);
                const Crucible::Vec2& rayEndPositionRightOfVertex = Crucible::Vec2(rayEndVert.position.x + RAY_DELTA,
                        rayEndVert.position.y);

                // Scale rays
                Crucible::Vec2 entityPosition{entityTransform.position->x, entityTransform.position->y};
                Crucible::Vec2 endVa = ((rayEndPositionLeftOfVertex - entityPosition) * RAY_SCALE) + entityPosition;
                Crucible::Vec2 endVb = ((rayEndPositionRightOfVertex - entityPosition) * RAY_SCALE) + entityPosition;

                const float angleARads = std::atan2(endVa.y, endVa.x) + RADIANS_OFFSET;
                const float angleBRads = std::atan2(endVb.y, endVb.x) - RADIANS_OFFSET;

                Crucible::Vec2 endVResultA(endVa.x + std::cos(angleARads), endVa.y + std::sin(angleARads));
                Crucible::Vec2 endVResultB(endVb.x + std::cos(angleBRads), endVb.y + std::sin(angleBRads));

                const size_t additionalRayIndex = objectIndex + additionalRayGroupStartIdx;
                rayGroup.rays[additionalRayIndex] = Crucible::Ray(entityTransform.position, endVResultA);
                rayGroup.rays[additionalRayIndex + 1] = Crucible::Ray(entityTransform.position, endVResultB);
            }

            additionalRayGroupStartIdx += (TOTAL_ADDITIONAL_RAYS_FOR_OBJECT + TOTAL_ADDITIONAL_RAYS_PER_VERT);
        }
    }
}
