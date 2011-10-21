#ifndef _PROCESSORCONTROLUPDATEINTERFACE_H
#define _PROCESSORCONTROLUPDATEINTERFACE_H

class ProcessorControl;

class ProcessorControlUpdateInterface 
{
public:
    virtual void handleModelChanged(ProcessorControl* newModel) =0;
};

#endif
