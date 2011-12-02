#ifndef _MODELCONTROLSERVICE_H
#define _MODELCONTROLSERVICE_H
#include "serviceprovider.h"
#include "model/ModelControl.h"

class ModelControlService : public ServiceProvider
{
public:
    ModelControlService(ModelControl& mc)
        : modelControl(mc)
    { }

    virtual MethodList* getMethodList();
    virtual CallResult callMethod(char* method, MarshaledData* arguments, MarshaledData** result);

private:
    ModelControl& modelControl;
};

void registerModelControlService(ModelControl& mc);

#endif
