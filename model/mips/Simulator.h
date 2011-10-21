#ifndef _SIMULATOR_H
#define _SIMULATOR_H

/**
 * Header file with generic simulator typedefs, classes etc. usable for
 * all simulators.
 */

class ProcessorModel {
public:
    virtual void executeInstruction() =0;
};

#endif
