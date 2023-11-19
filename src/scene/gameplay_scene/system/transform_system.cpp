#include "transform_system.h"

TransformSystem::TransformSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
}

void TransformSystem::execute()
{

}