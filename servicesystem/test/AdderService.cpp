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

#include "AdderService.hpp"
#include "string.h"
#include "stdlib.h"

MethodList* AdderService::getMethodList()
{
    int no_of_entries = 1;
    MethodList* list = (MethodList*)malloc(sizeof(MethodList)+no_of_entries*sizeof(MethodSignature));
    list->size = no_of_entries;
    list->methods[0] = (MethodSignature*)malloc(sizeof(MethodSignature));
    list->methods[0]->name = "add";
    list->methods[0]->signature = "ii";
    list->methods[0]->returntype = "i";
    return list;
}

CallResult AdderService::callMethod(char* method, MarshaledData* arguments, MarshaledData** result)
{
    CallResult call_result = CALL_UNKNOWN_NAME;
    MarshallParser* parser = create_parser(arguments);

    if ((strncmp(method,"add",3)==0))
    {
        if (strncmp(arguments->typelist,"ii",2)==0)
        {
            call_result = CALL_OK;
            /* Parse the parameters */
            int param1, param2;
            if (parse_int(parser, &param1)!=PARSING_OK)
            {
                call_result = CALL_MARSHALLING_ERROR;
                goto clean_up;
            }
            if (parse_int(parser, &param2)!=PARSING_OK)
            {
                call_result = CALL_MARSHALLING_ERROR;
                goto clean_up;
            }

            /* execute the function */
            int service_result = param1 + param2;
            /* Create the result structure */
            *result = create_method_call();
            append_int(*result, service_result);
        } else
        {
            call_result = CALL_UNKNOWN_SIGNATURE;
        }
    }

clean_up:
    free_parser(parser);
    return call_result;
}

