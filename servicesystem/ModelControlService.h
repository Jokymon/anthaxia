#ifndef _MODELCONTROLSERVICE_H
#define _MODELCONTROLSERVICE_H
#include "serviceprovider.h"

class ModelControlService : public ServiceProvider
{
public:
    ModelControlService() { }

    virtual MethodList* getMethodList();
    virtual CallResult callMethod(char* method, MarshaledData* arguments, MarshaledData** result);
};

void registerModelControlService();

#endif
