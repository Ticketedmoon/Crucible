#include "ray_appender_system.h"

RayAppenderSystem::RayAppenderSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void RayAppenderSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (auto& entity : entities)
    {
        if (!entity.hasComponent<Component::CLightSource>())
        {
            continue;
        }

        auto& playerLightSource = entity.getComponent<Component::CLightSource>();
        auto& playerTransform = entity.getComponent<Component::CTransform>();

        int otherShapeIndex = 0;
        for (std::shared_ptr<sf::VertexArray>& objectVertices : LevelManager::activeLevel.layers[0].tileObjectsVertexLayer)
        {
            const size_t totalShapeVertices = objectVertices->getVertexCount();
            const size_t totalAdditionalRaysPerEntity = totalShapeVertices * totalRaysPerVertex;

            for (size_t vertIndex = 0; vertIndex < totalAdditionalRaysPerEntity; vertIndex += totalRaysPerVertex)
            {
                const sf::Vertex& v = (*objectVertices)[vertIndex/totalRaysPerVertex];
                
                // Update ray vector to point left and right of endV with large RAY_SCALE.
                const size_t additionalRayIndex = TOTAL_CORE_RAYS + vertIndex + (totalAdditionalRaysPerEntity * otherShapeIndex);

                const Crucible::Vec2& rayEndPositionLeftOfVertex = Crucible::Vec2(v.position.x-RAY_DELTA, v.position.y);
                const Crucible::Vec2& rayEndPositionRightOfVertex = Crucible::Vec2(v.position.x+RAY_DELTA, v.position.y);

                // Scale rays
                Crucible::Vec2 playerPosition{playerTransform.position->x, playerTransform.position->y};
                Crucible::Vec2 endVa = ((rayEndPositionLeftOfVertex - playerPosition) * RAY_SCALE) + playerPosition;
                Crucible::Vec2 endVb = ((rayEndPositionRightOfVertex - playerPosition) * RAY_SCALE) + playerPosition;

                const float angleARads = std::atan2(endVa.y, endVa.x) + RADIANS_OFFSET;
                const float angleBRads = std::atan2(endVb.y, endVb.x) - RADIANS_OFFSET;

                Crucible::Vec2 endVResultA(endVa.x + std::cos(angleARads), endVa.y + std::sin(angleARads));
                Crucible::Vec2 endVResultB(endVb.x + std::cos(angleBRads), endVb.y + std::sin(angleBRads));

                playerLightSource.rays[additionalRayIndex] = Crucible::Ray(playerTransform.position, endVResultA);
                playerLightSource.rays[additionalRayIndex+1] = Crucible::Ray(playerTransform.position, endVResultB);
            }

            otherShapeIndex++;
        }
    }
}
