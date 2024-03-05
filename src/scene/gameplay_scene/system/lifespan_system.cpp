#include "lifespan_system.h"

LifespanSystem::LifespanSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LifespanSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntitiesByComponentType<Component::CLifeSpan>();
    for (Entity& e : entities)
    {
        auto& cLifeSpan = e.getComponent<Component::CLifeSpan>();
        if (cLifeSpan.ttlFrames == 0)
        {
            e.destroy();
        }
        else
        {
            cLifeSpan.ttlFrames--;
        }
    }
}
