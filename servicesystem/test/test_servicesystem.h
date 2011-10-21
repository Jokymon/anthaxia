#ifndef _TEST_SERVICESYSTEM_H
#define _TEST_SERVICESYSTEM_H

#include <cppunit/extensions/HelperMacros.h>
#include "servicesystem/servicesystem.h"
#include "servicesystem/serviceprovider.h"

class ServiceSystemTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE( ServiceSystemTest );
    CPPUNIT_TEST( testEmptyServiceRegistry );
    CPPUNIT_TEST( testServiceRegistration );
    CPPUNIT_TEST( testUsingService );
    CPPUNIT_TEST( testNonexistentMethod );
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testEmptyServiceRegistry();
    void testServiceRegistration();
    void testUsingService();
    void testNonexistentMethod();
private:
    ServiceSystem* srvsys;
};

#endif
