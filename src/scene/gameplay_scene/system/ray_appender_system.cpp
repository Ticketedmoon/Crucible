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
        for (auto & otherEntity : entities)
        {
            if (entity.getId() == otherEntity.getId())
            {
                continue;
            }

            const auto& entityRectangleShape = otherEntity.getComponent<Component::CShape>();
            const size_t totalShapeVertices = (entityRectangleShape.vertices.getVertexCount() - 1);
            const size_t totalAdditionalRaysPerEntity = totalShapeVertices * totalRaysPerVertex;

            for (size_t vertIndex = 0; vertIndex < totalAdditionalRaysPerEntity; vertIndex+= totalRaysPerVertex)
            {
                const sf::Vertex& v = entityRectangleShape.vertices[vertIndex/totalRaysPerVertex];
                
                // Update ray vector to point left and right of endV with large RAY_SCALE.
                const size_t additionalRayIndex = TOTAL_CORE_RAYS + vertIndex + (totalAdditionalRaysPerEntity * otherShapeIndex);

                const Crucible::Vec2& rayEndPositionLeftOfVertexA = Crucible::Vec2(v.position.x-RAY_DELTA, v.position.y);
                const Crucible::Vec2& rayEndPositionRightOfVertexA = Crucible::Vec2(v.position.x+RAY_DELTA, v.position.y);
                const Crucible::Vec2& rayEndPositionLeftOfVertexB = Crucible::Vec2(v.position.x, v.position.y-RAY_DELTA);
                const Crucible::Vec2& rayEndPositionRightOfVertexB = Crucible::Vec2(v.position.x, v.position.y+RAY_DELTA);

                // Scale rays
                // @document
                // @Review: There is likely a better way to achieve this with just two additional rays rather than 4.
                //            Instead of modifying the x and y values directly, we can just apply vector addition to
                //            slightly alter the direction of the new rays, which will work for any vertex.
                Crucible::Vec2 playerPosition{playerTransform.position->x, playerTransform.position->y};
                Crucible::Vec2 endVa = ((rayEndPositionLeftOfVertexA - playerPosition) * RAY_SCALE) + playerPosition;
                Crucible::Vec2 endVb = ((rayEndPositionRightOfVertexA - playerPosition) * RAY_SCALE) + playerPosition;
                Crucible::Vec2 endVc = ((rayEndPositionLeftOfVertexB - playerPosition) * RAY_SCALE) + playerPosition;
                Crucible::Vec2 endVd = ((rayEndPositionRightOfVertexB - playerPosition) * RAY_SCALE) + playerPosition;

                // @Review: Look to improve on this - update rays each frame in-place rather than new object creation.
                playerLightSource.rays[additionalRayIndex] = Crucible::Ray(playerTransform.position, endVa);
                playerLightSource.rays[additionalRayIndex+1] = Crucible::Ray(playerTransform.position, endVb);
                playerLightSource.rays[additionalRayIndex+2] = Crucible::Ray(playerTransform.position, endVc);
                playerLightSource.rays[additionalRayIndex+3] = Crucible::Ray(playerTransform.position, endVd);
            }

            otherShapeIndex++;
        }
    }
}
