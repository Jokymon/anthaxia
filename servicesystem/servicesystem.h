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
