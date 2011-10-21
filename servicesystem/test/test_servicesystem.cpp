#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include "AdderService.hpp"
#include "test_servicesystem.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ServiceSystemTest );

void ServiceSystemTest::setUp()
{
    srvsys = ServiceSystem::getServiceSystem();
}

void ServiceSystemTest::tearDown()
{
}

void ServiceSystemTest::testEmptyServiceRegistry()
{
    CPPUNIT_ASSERT(srvsys!=NULL);
    // Test the plain C interface
    CPPUNIT_ASSERT(get_service("test_service") == NULL);
    // and the CPP interface
    CPPUNIT_ASSERT(srvsys->getService("test_service") == NULL);
}

void ServiceSystemTest::testServiceRegistration()
{
    CPPUNIT_ASSERT(srvsys!=NULL);
    AdderService* adder_srv = new AdderService();
    // Register the new service
    srvsys->registerService("adder_service", adder_srv);
    // This should now be available
    CPPUNIT_ASSERT(srvsys->getService("adder_service") == adder_srv);

    // Test unregistering
    srvsys->unregisterService("adder_service");
    // The service should not be available anymore
    CPPUNIT_ASSERT(srvsys->getService("adder_service") == NULL);
}

void ServiceSystemTest::testUsingService()
{
    CPPUNIT_ASSERT(srvsys!=NULL);
    // Register the service
    AdderService* adder_srv = new AdderService();
    srvsys->registerService("adder_service", adder_srv);

    // Use the service
    ProxyObject po_adder = get_service("adder_service");
    CPPUNIT_ASSERT(po_adder!=NULL);

    MarshaledData* args = create_method_call();
    MarshaledData* result;
    append_int(args, 22);
    append_int(args, 20);
    CallResult res = call_method(po_adder, (char*)"add", args, &result);

    // Extract result
    CPPUNIT_ASSERT(res==CALL_OK);
    MarshallParser* parser = create_parser(result);
    int result_value;
    CPPUNIT_ASSERT(parse_int(parser, &result_value)==PARSING_OK);
    CPPUNIT_ASSERT(result_value==42);

    // Cleanup
    free_parser(parser);
    free_method_call(result);
    free_method_call(args);
    srvsys->unregisterService("adder_service");
}

void ServiceSystemTest::testNonexistentMethod()
{
    CPPUNIT_ASSERT(srvsys!=NULL);
    // Register the service
    AdderService* adder_srv = new AdderService();
    srvsys->registerService("adder_service", adder_srv);

    // Use the service
    ProxyObject po_adder = get_service("adder_service");
    CPPUNIT_ASSERT(po_adder!=NULL);

    MarshaledData* args = create_method_call();
    MarshaledData* result = NULL;
    append_int(args, 22);
    append_int(args, 20);
    CallResult res = call_method(po_adder, (char*)"invalid_method", args, &result);

    // This is a somehow stupid test as the return value is produced by the
    // AdderService object that is part of the test code
    CPPUNIT_ASSERT(res==CALL_UNKNOWN_NAME);
    CPPUNIT_ASSERT(result==NULL);
    free_method_call(args);

    args = create_method_call();
    append_double(args, 23.23);
    res = call_method(po_adder, (char*)"add", args, &result);

    // This is a somehow stupid test as the return value is produced by the
    // AdderService object that is part of the test code
    CPPUNIT_ASSERT(res==CALL_UNKNOWN_SIGNATURE);
    CPPUNIT_ASSERT(result==NULL);

    // Cleanup
    free_method_call(args);
    srvsys->unregisterService("adder_service");
}

int main(int argc, char* argv[]) {
    CppUnit::Test* test =
        CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextTestRunner runner;
    runner.addTest(test);

    bool wasSuccessful = runner.run();
    return !wasSuccessful;  // inverted because of command.com has different meaning
}
