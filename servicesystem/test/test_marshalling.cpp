#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <stdlib.h>
#include <string.h>
#include "test_marshalling.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MarshallingTest );

void MarshallingTest::setUp()
{ }

void MarshallingTest::tearDown()
{ }

void MarshallingTest::testIntMarshalling()
{
    MarshaledData* data = create_method_call();

    append_int(data, 1);
    append_int(data, 23);
    append_int(data, 454);
    append_int(data, 32);
    append_int(data, 1);

    int result;
    MarshallParser* parser = create_parser(data);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    CPPUNIT_ASSERT(result==1);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    CPPUNIT_ASSERT(result==23);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    CPPUNIT_ASSERT(result==454);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    CPPUNIT_ASSERT(result==32);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    CPPUNIT_ASSERT(result==1);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_NONE);

    free_parser(parser);
    free_method_call(data);
}

void MarshallingTest::testDoubleMarshalling()
{
    MarshaledData* data = create_method_call();

    append_double(data, 3.141);
    append_double(data, 999.999);
    append_double(data, 0.0);

    double result;
    MarshallParser* parser = create_parser(data);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_DOUBLE);
    parse_double(parser, &result);
    CPPUNIT_ASSERT(result==3.141);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_DOUBLE);
    parse_double(parser, &result);
    CPPUNIT_ASSERT(result==999.999);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_DOUBLE);
    parse_double(parser, &result);
    CPPUNIT_ASSERT(result==0.0);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_NONE);

    free_parser(parser);
    free_method_call(data);
}

void MarshallingTest::testStringMarshalling()
{
    MarshaledData* data = create_method_call();

    append_string(data, "hello");
    append_string(data, "world");
    append_string(data, "");

    MarshallParser* parser = create_parser(data);

    char* result;
    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_STRING);
    parse_string(parser, &result);
    CPPUNIT_ASSERT(strncmp("hello", result, 5)==0);
    free(result);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_STRING);
    parse_string(parser, &result);
    CPPUNIT_ASSERT(strncmp("world", result, 5)==0);
    free(result);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_STRING);
    parse_string(parser, &result);
    CPPUNIT_ASSERT(strlen(result)==0);
    free(result);

    CPPUNIT_ASSERT(get_next_type(parser)==TYPE_NONE);

    free_parser(parser);
    free_method_call(data);
}

int main(int argc, char* argv[]) {
    CppUnit::Test* test =
        CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextTestRunner runner;
    runner.addTest(test);

    bool wasSuccessful = runner.run();
    return !wasSuccessful;  // inverted because of command.com has different meaning
}
