#include "SimulationObject.h"

SimulationObject::SimulationObject(std::string _name, SimulationObjectTypes _type, SimulationObject* _parent)
: name(_name), object_type(_type), parent(_parent)
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

SimulationObject* SimulationObject::getParent()
{
    return parent;
}

void SimulationObject::setParent(SimulationObject* _parent)
{
    parent = _parent;
}

