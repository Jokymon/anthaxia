#ifndef _SYSTEM_ON_CHIP_H
#define _SYSTEM_ON_CHIP_H

#include "SimulationObject.h"

class SystemOnChip : public SimulationObject
{
public:
    SystemOnChip(std::string _name);

    virtual void accept(SimulationObjectVisitor& visitor);
};

#endif
