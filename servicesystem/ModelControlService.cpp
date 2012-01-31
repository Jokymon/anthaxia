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

#include "ModelControlService.h"
#include "string.h"
#include "stdlib.h"

MethodList* ModelControlService::getMethodList()
{
    CREATE_METHOD_LIST(list, 9)
    SET_LIST_ITEM(list, 0, "getPC",             "",   "i")
    SET_LIST_ITEM(list, 1, "getRegister",       "i",  "i")
    SET_LIST_ITEM(list, 2, "setRegister",       "ii", "i")
    SET_LIST_ITEM(list, 3, "getRegisterCount",  "",   "i")
    SET_LIST_ITEM(list, 4, "start",             "",   "")
    SET_LIST_ITEM(list, 5, "step",              "",   "")
    SET_LIST_ITEM(list, 6, "stop",              "",   "")
    SET_LIST_ITEM(list, 7, "reset",             "",   "")
    SET_LIST_ITEM(list, 8, "loadImage",         "s",  "")

    return list;
}

CallResult ModelControlService::callMethod(char* method, MarshaledData* arguments, MarshaledData** result)
{
    CallResult call_result = CALL_UNKNOWN_NAME;
    MarshallParser* parser = create_parser(arguments);

    if (strncmp(method, "getPC", 5)==0)
    {
        call_result = CALL_OK;
        int intresult = modelControl.getPC();
        /* Create result marshall */
        *result = create_method_call();
        append_int(*result, intresult);
    } else if (strncmp(method, "getRegisterCount", 16)==0)
    {
        call_result = CALL_OK;
        int intresult = modelControl.getRegisterCount();
        /* Create result marshall */
        *result = create_method_call();
        append_int(*result, intresult);
    } else if (strncmp(method, "getRegister", 11)==0)
    {
        if (strncmp(arguments->typelist,"i",1)==0)
        {
            call_result = CALL_OK;
            int param1;
            if (parse_int(parser, &param1)!=PARSING_OK)
            {
                call_result = CALL_MARSHALLING_ERROR;
                goto clean_up;
            }
            int intresult;
            bool success = modelControl.getRegister(param1, intresult);
            *result = create_method_call();
            if (success)
            {
                append_int(*result, intresult);
            }
        }
    } else if (strncmp(method, "setRegister", 11)==0)
    {
        if (strncmp(arguments->typelist,"ii",2)==0)
        {
            call_result = CALL_OK;
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
            bool success = modelControl.setRegister(param1, param2);
            /* Create result marshall */
            *result = create_method_call();
            append_int(*result, (int)success);
        }
    } else if (strncmp(method, "start", 5)==0)
    {
        call_result = CALL_OK;
        modelControl.startSimulation();
        *result = create_method_call();
        // append nothing, return value is void
    } else if (strncmp(method, "step", 4)==0)
    {
        call_result = CALL_OK;
        modelControl.stepSimulation();
        *result = create_method_call();
        // append nothing, return value is void
    } else if (strncmp(method, "stop", 4)==0)
    {
        call_result = CALL_OK;
        modelControl.stopSimulation();
        *result = create_method_call();
        // append nothing, return value is void
    } else if (strncmp(method, "reset", 5)==0)
    {
        call_result = CALL_OK;
        modelControl.resetSimulation();
        *result = create_method_call();
        // append nothing, return value is void
    } else if (strncmp(method, "loadImage", 9)==0)
    {
        if (strncmp(arguments->typelist,"s",1)==0)
        {
            call_result = CALL_OK;
            char* param1;
            if (parse_string(parser, &param1)!=PARSING_OK)
            {
                call_result = CALL_MARSHALLING_ERROR;
                goto clean_up;
            }
            modelControl.loadImage(std::string(param1));
            *result = create_method_call();
        }
    }

clean_up:
    free_parser(parser);
    return call_result;
}

void registerModelControlService(ModelControl& mc)
{
    ServiceSystem* sys = ServiceSystem::getServiceSystem();
    ModelControlService* mcs = new ModelControlService(mc);
    sys->registerService("modelcontrol", mcs);
}

