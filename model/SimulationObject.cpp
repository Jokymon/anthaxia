#include "SimulationObject.h"

SimulationObject::SimulationObject(std::string _name, SimulationObjectTypes _type)
: name(_name), object_type(_type)
{ }

std::string SimulationObject::getName()
{
    return name;
}

SimulationObjectTypes SimulationObject::getObjectType()
{
    return object_type;
}

SimulationObject* SimulationObject::find(std::string name)
{
}

bool SimulationObject::appendChild(SimulationObject* child)
{
    return false;
}

