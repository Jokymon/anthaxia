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

#include "marshalling.h"
#include <stdlib.h>
#include <string.h>

#define BUFFER_GRANULARITY 16
#define TYPE_GRANULARITY 4

MarshaledData* create_method_call()
{
    MarshaledData* meth = (MarshaledData*)malloc(sizeof(MarshaledData));
    meth->buffer = malloc(BUFFER_GRANULARITY);
    meth->buffersize = 0;
    meth->bufferavailable = BUFFER_GRANULARITY;
    meth->typelist = (char*)malloc(TYPE_GRANULARITY); 
    meth->typelistsize = 0;
    meth->typelistavailable = TYPE_GRANULARITY;
    return meth;
}

void free_method_call(MarshaledData* meth)
{
    free(meth->buffer);
    free(meth->typelist);
    free(meth);
}

void append_int(MarshaledData* meth, int value)
{
    // Make sure the buffer has enough space to add an integer value
    if (meth->bufferavailable - meth->buffersize < sizeof(int))
    {
        meth->buffer = realloc(meth->buffer, meth->bufferavailable + BUFFER_GRANULARITY);
        meth->bufferavailable += BUFFER_GRANULARITY;
    }
    // Make sure the typelist has enough space to add an integer marker
    if (meth->typelistavailable - meth->typelistsize < 1)
    {
        meth->typelist = (char*)realloc(meth->typelist, meth->typelistavailable + TYPE_GRANULARITY);
        meth->typelistavailable += TYPE_GRANULARITY;
    }

    *((int*)(meth->buffer + meth->buffersize)) = value;
    meth->buffersize += sizeof(int);
    meth->typelist[meth->typelistsize] = 'i';
    meth->typelistsize += 1;
}

void append_double(MarshaledData* meth, double value)
{
    // Make sure the buffer has enough space to add an integer value
    if (meth->bufferavailable - meth->buffersize < sizeof(double))
    {
        meth->buffer = realloc(meth->buffer, meth->bufferavailable + BUFFER_GRANULARITY);
        meth->bufferavailable += BUFFER_GRANULARITY;
    }
    // Make sure the typelist has enough space to add an integer marker
    if (meth->typelistavailable - meth->typelistsize < 1)
    {
        meth->typelist = (char*)realloc(meth->typelist, meth->typelistavailable + TYPE_GRANULARITY);
        meth->typelistavailable += TYPE_GRANULARITY;
    }

    *((double*)(meth->buffer + meth->buffersize)) = value;
    meth->buffersize += sizeof(double);
    meth->typelist[meth->typelistsize] = 'd';
    meth->typelistsize += 1;
}

void append_string(MarshaledData* meth, char* value)
{
    size_t len = strlen(value)+1;
    // Make sure the buffer has enough space to add the given string
    if (meth->bufferavailable - meth->buffersize <= len)
    {
        int addedsize = (len/BUFFER_GRANULARITY) + BUFFER_GRANULARITY - (len%BUFFER_GRANULARITY);
        meth->buffer = realloc(meth->buffer, meth->bufferavailable + addedsize);
        meth->bufferavailable += addedsize;
    }
    // Make sure the typelist has enough space to add the given string
    if (meth->typelistavailable - meth->typelistsize < 1+sizeof(size_t))
    {
        int addedsize = ((1+sizeof(size_t))/TYPE_GRANULARITY) + TYPE_GRANULARITY - ((1+sizeof(size_t))%TYPE_GRANULARITY);
        meth->typelist = (char*)realloc(meth->typelist, meth->typelistavailable + addedsize);
        meth->typelistavailable += TYPE_GRANULARITY;
    }

    memcpy(meth->buffer + meth->buffersize, value, len);
    meth->buffersize += len;
    meth->typelist[meth->typelistsize] = 's';
    *((size_t*)(meth->typelist+meth->typelistsize+1)) = len;
    meth->typelistsize += 1+sizeof(size_t);
}

void append_proxy_object(MarshaledData* meth, ProxyObject value)
{
    // Make sure the buffer has enough space to add an integer value
    if (meth->bufferavailable - meth->buffersize < sizeof(ProxyObject))
    {
        meth->buffer = realloc(meth->buffer, meth->bufferavailable + BUFFER_GRANULARITY);
        meth->bufferavailable += BUFFER_GRANULARITY;
    }
    // Make sure the typelist has enough space to add a proxy object marker
    if (meth->typelistavailable - meth->typelistsize < 1)
    {
        meth->typelist = (char*)realloc(meth->typelist, meth->typelistavailable + TYPE_GRANULARITY);
        meth->typelistavailable += TYPE_GRANULARITY;
    }

    *((ProxyObject*)(meth->buffer + meth->buffersize)) = value;
    meth->buffersize += sizeof(ProxyObject);
    meth->typelist[meth->typelistsize] = 'O';
    meth->typelistsize += 1;
}

MarshallParser* create_parser(MarshaledData* method)
{
    MarshallParser* parser = (MarshallParser*)malloc(sizeof(MarshallParser));
    parser->method = method;
    parser->bufferposition = method->buffer;
    parser->typelistposition = method->typelist;
    return parser;
}

void free_parser(MarshallParser* parser)
{
    free(parser);
}

ServiceTypes get_next_type(MarshallParser* parser)
{
    if (parser->typelistposition -  parser->method->typelist >= parser->method->typelistsize)
        return TYPE_NONE;
    switch (*parser->typelistposition)
    {
        case 'i':
            return TYPE_INT;
        case 'd':
            return TYPE_DOUBLE;
        case 's':
            return TYPE_STRING;
        case 'O':
            return TYPE_OBJECT;
        default:
            return TYPE_NONE;
    }
}

ParsingResult parse_int(MarshallParser* parser, int* result)
{
    // TODO: check that we still have enough parameters
    if (*parser->typelistposition != 'i')
        return TYPE_MISMATCH;
    parser->typelistposition++;
    *result = *((int*)(parser->bufferposition));
    parser->bufferposition += sizeof(int);
    return PARSING_OK;
}

ParsingResult parse_double(MarshallParser* parser, double* result)
{
    // TODO: check that we still have enough parameters
    if (*parser->typelistposition != 'd')
        return TYPE_MISMATCH;
    parser->typelistposition++;
    *result = *((double*)(parser->bufferposition));
    parser->bufferposition += sizeof(double);
    return PARSING_OK;
}

ParsingResult parse_string(MarshallParser* parser, char** result)
{
    // TODO: serious type checking needed
    if (*parser->typelistposition != 's')
        return TYPE_MISMATCH;
    size_t len = *((size_t*)(parser->typelistposition+1));
    *result = (char*)malloc(len);
    memcpy(*result, parser->bufferposition, len);
    parser->typelistposition += 1+sizeof(size_t);
    parser->bufferposition += len;
    return PARSING_OK;
}

ParsingResult parse_proxy_object(MarshallParser* parser, ProxyObject* result)
{
    // TODO: serious type checking needed
    if (*parser->typelistposition != 'O')
        return TYPE_MISMATCH;
    parser->typelistposition++;
    *result = *((ProxyObject*)(parser->bufferposition));
    parser->bufferposition += sizeof(ProxyObject);
    return PARSING_OK;
}

