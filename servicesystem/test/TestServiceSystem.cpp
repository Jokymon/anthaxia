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

#include "servicesystem/servicesystem.h"
#include "servicesystem/serviceprovider.h"

#include "AdderService.hpp"
#include "gtest/gtest.h"

namespace {

class TestServiceSystem : public ::testing::Test 
{
protected:
    TestServiceSystem() 
    { }

    virtual void SetUp()
    {
        srvsys = ServiceSystem::getServiceSystem();
    }

    virtual void TearDown()
    { }

    ServiceSystem* srvsys;
};

TEST_F(TestServiceSystem, EmptyServiceRegistry)
{
    ASSERT_TRUE(srvsys!=NULL);
    // Test the plain C interface
    ASSERT_TRUE(get_service("test_service") == NULL);
    // and the CPP interface
    ASSERT_TRUE(srvsys->getService("test_service") == NULL);
}

}

TEST_F(TestServiceSystem, ServiceRegistration)
{
    ASSERT_TRUE(srvsys!=NULL);
    AdderService* adder_srv = new AdderService();
    // Register the new service
    srvsys->registerService("adder_service", adder_srv);
    // This should now be available
    ASSERT_TRUE(srvsys->getService("adder_service") == adder_srv);

    // Test unregistering
    srvsys->unregisterService("adder_service");
    // The service should not be available anymore
    ASSERT_TRUE(srvsys->getService("adder_service") == NULL);
}

TEST_F(TestServiceSystem, UsingService)
{
    ASSERT_TRUE(srvsys!=NULL);
    // Register the service
    AdderService* adder_srv = new AdderService();
    srvsys->registerService("adder_service", adder_srv);

    // Use the service
    ProxyObject po_adder = get_service("adder_service");
    ASSERT_TRUE(po_adder!=NULL);

    MarshaledData* args = create_method_call();
    MarshaledData* result;
    append_int(args, 22);
    append_int(args, 20);
    CallResult res = call_method(po_adder, (char*)"add", args, &result);

    // Extract result
    ASSERT_TRUE(res==CALL_OK);
    MarshallParser* parser = create_parser(result);
    int result_value;
    ASSERT_TRUE(parse_int(parser, &result_value)==PARSING_OK);
    ASSERT_TRUE(result_value==42);

    // Cleanup
    free_parser(parser);
    free_method_call(result);
    free_method_call(args);
    srvsys->unregisterService("adder_service");
}

TEST_F(TestServiceSystem, NonexistentMethod)
{
    ASSERT_TRUE(srvsys!=NULL);
    // Register the service
    AdderService* adder_srv = new AdderService();
    srvsys->registerService("adder_service", adder_srv);

    // Use the service
    ProxyObject po_adder = get_service("adder_service");
    ASSERT_TRUE(po_adder!=NULL);

    MarshaledData* args = create_method_call();
    MarshaledData* result = NULL;
    append_int(args, 22);
    append_int(args, 20);
    CallResult res = call_method(po_adder, (char*)"invalid_method", args, &result);

    // This is a somehow stupid test as the return value is produced by the
    // AdderService object that is part of the test code
    ASSERT_TRUE(res==CALL_UNKNOWN_NAME);
    ASSERT_TRUE(result==NULL);
    free_method_call(args);

    args = create_method_call();
    append_double(args, 23.23);
    res = call_method(po_adder, (char*)"add", args, &result);

    // This is a somehow stupid test as the return value is produced by the
    // AdderService object that is part of the test code
    ASSERT_TRUE(res==CALL_UNKNOWN_SIGNATURE);
    ASSERT_TRUE(result==NULL);

    // Cleanup
    free_method_call(args);
    srvsys->unregisterService("adder_service");
}

