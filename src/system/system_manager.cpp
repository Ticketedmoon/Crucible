#include "system_manager.h"

void SystemManager::update(Crucible::GameProperties& gameProperties)
{
    for (std::shared_ptr<System>& system : m_systemsToUpdate)
    {
        if (system->canExecute(gameProperties))
        {
            system->execute();
        }
    }
}

void SystemManager::render()
{
    for (std::shared_ptr<System>& system : m_systemsToRender)
    {
        system->execute();
    }
}

void SystemManager::registerSystem(const std::shared_ptr<System>& system, SystemType type)
{
    if (type == SystemType::UPDATE)
    {
        m_systemsToUpdate.emplace_back(system);
    }
    else if (type == SystemType::RENDER)
    {
        m_systemsToRender.emplace_back(system);
    }
}
