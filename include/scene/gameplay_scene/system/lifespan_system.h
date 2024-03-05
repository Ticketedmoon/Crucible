#pragma once

#ifndef CRUCIBLE_LIFESPAN_SYSTEM_H
#define CRUCIBLE_LIFESPAN_SYSTEM_H


#include "system.h"
#include "entity_manager.h"

class LifespanSystem : public System
{
    public:
        explicit LifespanSystem(EntityManager& entityManager);

        void execute() override;

    private:
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_LIFESPAN_SYSTEM_H
