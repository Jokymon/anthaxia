#ifndef _ADDERSERVICE_HPP
#define _ADDERSERVICE_HPP
#include "servicesystem/serviceprovider.h"

class AdderService : public ServiceProvider
{
public:
    AdderService() 
    {
    }

    virtual MethodList* getMethodList();
    virtual CallResult callMethod(char* method, MarshaledData* arguments, MarshaledData** result);
};

#endif
