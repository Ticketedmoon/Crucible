#pragma once

#ifndef CRUCIBLE_SYSTEM_H
#define CRUCIBLE_SYSTEM_H

#include "common_properties.h"

class System
{
    public:
        virtual ~System() = default;
        virtual void execute() = 0;
        virtual bool canExecute(const Crucible::GameProperties& gameProperties)
        {
            return true;
        }
};

#endif //CRUCIBLE_SYSTEM_H
