#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include "SimulationObject.h"

class Processor : public SimulationObject
{
public:
    Processor(std::string _name);

    virtual void run() =0;
    virtual void step() =0;
    virtual void stop() =0;
    virtual void reset() =0;


public: // signals
    sigslot::signal0<> SimulationStarted;
    sigslot::signal0<> SimulationStopped;
    sigslot::signal0<> SimulationReset;
};

#endif
