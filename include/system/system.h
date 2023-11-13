#pragma once

#ifndef CRUCIBLE_SYSTEM_H
#define CRUCIBLE_SYSTEM_H

class System
{
    public:
        virtual ~System() = default;
        virtual void execute() = 0;
};

#endif //CRUCIBLE_SYSTEM_H
