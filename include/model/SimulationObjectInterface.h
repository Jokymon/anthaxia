#ifndef _SIMULATION_OBJECT_INTERFACE_H
#define _SIMULATION_OBJECT_INTERFACE_H

#include "sigslot/sigslot.h"
#include <string>

enum SimulationObjectTypes {
    SO_SOC,
    SO_PROCESSOR,
    SO_BREAKPOINTS,
    SO_REGISTERBANK,
    SO_MEMORY,
};

class SimulationObjectVisitor;

class SimulationObjectInterface
{
public:
    SimulationObjectInterface(std::string _name, SimulationObjectTypes _type, SimulationObject* _parent);

    std::string getName() =0;
    SimulationObjectTypes getObjectType() =0;

    SimulationObject* find(std::string name) =0;

    SimulationObject* getParent() =0;
    void setParent(SimulationObject* _parent) =0;
    virtual int parentIndex() =0;
    virtual SimulationObject* getChild(int index) =0;
    virtual int childCount() =0;

// visitor support functions
    virtual void accept(SimulationObjectVisitor& visitor) =0;

public: // signals
    sigslot::signal0<> ObjectDestroyed;
};

#endif
