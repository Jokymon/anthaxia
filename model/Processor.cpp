#include "Processor.h"
#include "SimulationObjectVisitor.h"

Processor::Processor(std::string _name, SimulationObject* _parent)
: SimulationObject(_name, SO_PROCESSOR, _parent)
{ }

void Processor::accept(SimulationObjectVisitor& visitor)
{
    visitor.visit(*this);
}

