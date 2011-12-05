#include "SimulationObjectVisitor.h"

void SimulationObjectVisitor::visit(SimulationObject& so)
{
    so.accept(*this);
}
