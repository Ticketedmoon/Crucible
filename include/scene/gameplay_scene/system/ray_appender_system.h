#pragma once

#ifndef CRUCIBLE_RAY_APPENDER_SYSTEM_H
#define CRUCIBLE_RAY_APPENDER_SYSTEM_H


#include "system.h"
#include "entity_manager.h"

class RayAppenderSystem  : public System
{
    public:
        explicit RayAppenderSystem(EntityManager& entityManager);

        void execute() override;

    private:
        EntityManager& m_entityManager;

};


#endif //CRUCIBLE_RAY_APPENDER_SYSTEM_H
