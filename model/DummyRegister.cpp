#include "DummyRegister.h"
#include "SimulationObjectVisitor.h"

DummyRegister::DummyRegister(std::string name, SimulationObject* _parent)
: SimulationObject(name, SO_REGISTERBANK, _parent)
{ }

int DummyRegister::parentIndex()
{
    return 0;
}

SimulationObject* DummyRegister::getChild(int index)
{
}

int DummyRegister::childCount()
{
    return 0;
}

void DummyRegister::accept(SimulationObjectVisitor& visitor)
{
    visitor.visit(*this);
}
