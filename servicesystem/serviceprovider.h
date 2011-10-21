#ifndef _SERVICEPROVIDER_H
#define _SERVICEPROVIDER_H

#include "servicesystem.h"
#include <map>
#include <string>
#include <vector>

class ServiceProvider
{
public:
    virtual MethodList* getMethodList() =0;
    virtual CallResult callMethod(char* method, MarshaledData* arguments, MarshaledData** result) =0;
};

class ServiceSystem
{
public:
    static ServiceSystem* getServiceSystem();

    ServiceProvider* getService(std::string name);
    // The service registry takes ownership of the new service provider
    void registerService(std::string name, ServiceProvider* service);
    void unregisterService(std::string name);
    std::vector<std::string> getServiceList();

private:
    ServiceSystem();

    std::map<std::string, ServiceProvider*> mServiceRegistry;

    static ServiceSystem* theInstance;
};

#define CREATE_METHOD_LIST(VARIABLE, SIZE) \
    MethodList* VARIABLE = (MethodList*)malloc(sizeof(MethodList)+(SIZE)*sizeof(MethodSignature)); \
    VARIABLE->size = (SIZE);

#define SET_LIST_ITEM(VARIABLE, INDEX, NAME, ARGS, RETURN_TYPE) \
    VARIABLE->methods[(INDEX)] = (MethodSignature*)malloc(sizeof(MethodSignature)); \
    VARIABLE->methods[(INDEX)]->name = NAME; \
    VARIABLE->methods[(INDEX)]->signature = ARGS; \
    VARIABLE->methods[(INDEX)]->returntype = RETURN_TYPE;

#endif
