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

#ifndef _SERVICESYSTEM_H
#define _SERVICESYSTEM_H

#include "marshalling.h"

typedef struct {
    const char* name;
    const char* signature;
    const char* returntype;
} MethodSignature;

typedef struct {
    int size;
    MethodSignature* methods[];
} MethodList;

typedef struct {
    int size;
    char** services;
} ServiceList;

#ifdef __cplusplus
extern "C" {
#endif

/* request a service object with the given name */
ProxyObject get_service(const char* name);

/* get a list with the names of all available services */
ServiceList* get_service_list();

enum CallResultEnum { 
    CALL_OK,                /* call succeeded successfully */
    CALL_UNKNOWN_NAME,      /* a method with the given name could not be found */
    CALL_UNKNOWN_SIGNATURE, /* a method with the given method types could not be found */
    CALL_MARSHALLING_ERROR  /* An error happend during marshalling or demarshalling; this means that the marshalling code is erroneous */
};
typedef enum CallResultEnum CallResult;

MethodList* get_method_list(ProxyObject po);
CallResult call_method(ProxyObject po, char* method, MarshaledData* arguments, MarshaledData** result);

#ifdef __cplusplus
}
#endif

#endif
