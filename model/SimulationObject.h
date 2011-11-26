#ifndef _SIMULATION_OBJECT_H
#define _SIMULATION_OBJECT_H

#include "sigslot/sigslot.h"
#include <string>
#include <vector>

enum SimulationObjectTypes { 
    SO_PROCESSOR,
    SO_BREAKPOINTS,
    SO_REGISTERS,
    SO_MEMORY,
};

class SimulationObject
{
public:
    SimulationObject(std::string _name, SimulationObjectTypes _type);

    std::string getName();
    SimulationObjectTypes getObjectType();

    SimulationObject* find(std::string name);
    bool appendChild(SimulationObject* child);

public: // signals
    sigslot::signal0<> ObjectDestroyed;
    
private:
    std::string name;
    SimulationObjectTypes object_type;
    std::vector<SimulationObject*> children;
};

#endif
