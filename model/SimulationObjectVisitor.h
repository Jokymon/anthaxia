#ifndef _SIMULATION_OBJECT_VISITOR_H
#define _SIMULATION_OBJECT_VISITOR_H

#include "SimulationObject.h"
#include "Processor.h"
#include "RegisterBank.h"
#include "SystemOnChip.h"

class SimulationObjectVisitor
{
public:
    void visit(SimulationObject& so);

    virtual void visit(Processor& object) =0;
    virtual void visit(RegisterBank& object) =0;
    virtual void visit(SystemOnChip& object) =0;
};

#endif
