/***********************************************************************
 *
 * This file is part of Anthaxia.
 * 
 * Anthaxia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Anthaxia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Anthaxia.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

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
