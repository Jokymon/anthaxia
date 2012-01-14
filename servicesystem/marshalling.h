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

#ifndef _MARSHALLING_H
#define _MARSHALLING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Type passed around to identify a service object */
typedef void* ProxyObject;

/* structure to encapsulate a marshalled method call */
typedef struct {
    /* the buffer takes the marshalled data */
    void* buffer;   int buffersize;     int bufferavailable;
    /* the typelist takes the information about the marshalled types */
    char* typelist; int typelistsize;   int typelistavailable;
} MarshaledData;

/* enum with the types supported by the service system */
enum ServiceTypesEnum {
    TYPE_INT, 
    TYPE_DOUBLE, 
    TYPE_STRING, 
    TYPE_OBJECT, 
    TYPE_NONE 
};
typedef enum ServiceTypesEnum ServiceTypes;

/****************************************************************
 * functions for creating a marshalled method call
 */

MarshaledData* create_method_call();
void free_method_call(MarshaledData* meth);
void append_int(MarshaledData* meth, int value);
void append_double(MarshaledData* meth, double value);
void append_string(MarshaledData* meth, const char* value);
void append_proxy_object(MarshaledData* meth, ProxyObject value);

/****************************************************************
 * structures and functions for parsing a marshalled method call
 */

/* structure to encapsulate a marshalling parser */
typedef struct {
    MarshaledData* method;
    void* bufferposition;
    char* typelistposition;
} MarshallParser;

enum ParsingResultEnum { PARSING_OK, TYPE_MISMATCH, NO_MORE_PARAMETERS };
typedef enum ParsingResultEnum ParsingResult;

MarshallParser* create_parser(MarshaledData* method);
void free_parser(MarshallParser* parser);
ServiceTypes get_next_type(MarshallParser* parser);
ParsingResult parse_int(MarshallParser* parser, int* result);
ParsingResult parse_double(MarshallParser* parser, double* result);
ParsingResult parse_string(MarshallParser* parser, char** result);
ParsingResult parse_proxy_object(MarshallParser* parser, ProxyObject* value);

#ifdef __cplusplus
}
#endif

#endif
