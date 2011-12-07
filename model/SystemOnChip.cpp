#include "SystemOnChip.h"
#include "SimulationObjectVisitor.h"

SystemOnChip::SystemOnChip(std::string _name, SimulationObject* _parent)
: SimulationObject(_name, SO_SOC, _parent)
{ }

void SystemOnChip::accept(SimulationObjectVisitor& visitor)
{
    visitor.visit(*this);
}
