#ifndef _SIMULATION_OBJECT_H
#define _SIMULATION_OBJECT_H

#include "sigslot/sigslot.h"
#include <string>
#include <vector>

enum SimulationObjectTypes {
    SO_SOC,
    SO_PROCESSOR,
    SO_BREAKPOINTS,
    SO_REGISTERBANK,
    SO_MEMORY,
};

class SimulationObjectVisitor;

class SimulationObject
{
public:
    SimulationObject(std::string _name, SimulationObjectTypes _type, SimulationObject* _parent);

    std::string getName();
    SimulationObjectTypes getObjectType();

    SimulationObject* find(std::string name);

    SimulationObject* getParent();
    virtual int childCount() =0;
    virtual SimulationObject* getChild(int index) =0;

// visitor support functions
    virtual void accept(SimulationObjectVisitor& visitor) =0;

public: // signals
    sigslot::signal0<> ObjectDestroyed;
    
private:
    std::string name;
    SimulationObjectTypes object_type;
    SimulationObject* parent;
    std::vector<SimulationObject*> children;
};

#endif
