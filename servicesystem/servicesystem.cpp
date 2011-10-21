#include "servicesystem.h"
#include "serviceprovider.h"
#include "string.h"
#include "stdlib.h"

using namespace std;

ServiceSystem* ServiceSystem::getServiceSystem()
{ 
    if (theInstance==0) {
        theInstance = new ServiceSystem();
    }
    return theInstance; 
}

ServiceProvider* ServiceSystem::getService(std::string name) 
{
    map<std::string, ServiceProvider*>::iterator iter;

    iter = mServiceRegistry.find(name);
    if (iter!=mServiceRegistry.end())
    {
        return iter->second;
    } else
    {
        return 0;
    }
}

void ServiceSystem::registerService(std::string name, ServiceProvider* service)
{
    mServiceRegistry[name] = service;
}

std::vector<std::string> ServiceSystem::getServiceList()
{
    std::vector<std::string> service_list;
    for (map<std::string, ServiceProvider*>::iterator iter = mServiceRegistry.begin(); iter!=mServiceRegistry.end(); ++iter)
    {
        service_list.push_back(iter->first);
    }
    return service_list;
}


void ServiceSystem::unregisterService(std::string name)
{
    map<std::string, ServiceProvider*>::iterator iter;

    iter = mServiceRegistry.find(name);
    if (iter!=mServiceRegistry.end())
    {
        // We are the owner of the service, therefore we have to delete it
        delete iter->second;
        mServiceRegistry.erase(name);
    }
}

ServiceSystem::ServiceSystem()
{ }

ServiceSystem* ServiceSystem::theInstance = 0;

extern "C" {

ProxyObject get_service(const char* name)
{
    return (ProxyObject)ServiceSystem::getServiceSystem()->getService(std::string(name));
}

ServiceList* get_service_list()
{
    ServiceList* service_list = (ServiceList*)malloc(sizeof(ServiceList));
    std::vector<std::string> tmp = ServiceSystem::getServiceSystem()->getServiceList();
    service_list->size = tmp.size();
    service_list->services = new char*[tmp.size()];
    for (int i=0; i<tmp.size(); i++)
    {
        service_list->services[i] = (char*)tmp[i].c_str();
    }
    return service_list;
}

MethodList* get_method_list(ProxyObject po)
{
    return reinterpret_cast<ServiceProvider*>(po)->getMethodList();
}

CallResult call_method(ProxyObject po, char* method, MarshaledData* arguments, MarshaledData** result)
{
    return reinterpret_cast<ServiceProvider*>(po)->callMethod(method, arguments, result);
}

}

