#ifndef _TEST_MARSHALLING_H
#define _TEST_MARSHALLING_H

#include <cppunit/extensions/HelperMacros.h>
#include <servicesystem/marshalling.h>

class MarshallingTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE( MarshallingTest );
    CPPUNIT_TEST( testIntMarshalling );
    CPPUNIT_TEST( testDoubleMarshalling );
    CPPUNIT_TEST( testStringMarshalling );
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testIntMarshalling();
    void testDoubleMarshalling();
    void testStringMarshalling();
};

#endif
