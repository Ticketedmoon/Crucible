#pragma once

#ifndef CRUCIBLE_SYSTEM_MANAGER_H
#define CRUCIBLE_SYSTEM_MANAGER_H

#include <ranges>
#include <vector>
#include <memory>

#include "system.h"

class SystemManager
{
    public:
        enum class SystemType
        {
                UPDATE,
                RENDER
        };

        void update(Crucible::GameProperties& gameProperties);
        void render();

        void registerSystem(const std::shared_ptr<System>& system, SystemType type);

    private:
        std::vector<std::shared_ptr<System>> m_systemsToUpdate;
        std::vector<std::shared_ptr<System>> m_systemsToRender;
};


#endif //CRUCIBLE_SYSTEM_MANAGER_H
