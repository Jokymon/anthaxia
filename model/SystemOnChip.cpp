#include "SystemOnChip.h"
#include "SimulationObjectVisitor.h"

SystemOnChip::SystemOnChip(std::string _name)
: SimulationObject(_name, SO_SOC)
{ }

void SystemOnChip::accept(SimulationObjectVisitor& visitor)
{
    visitor.visit(*this);
}
