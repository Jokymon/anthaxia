# Testing the servicesystem
SET(TEST_SERVICESYSTEM_SRCS
    test_servicesystem.cpp
    AdderService.cpp
)

SET(TEST_MARSHALLING_SRCS
    test_marshalling.cpp
)

INCLUDE_DIRECTORIES(
    ${POCO_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}
)

LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES(
    cppunit
    servicesystem
)

ADD_EXECUTABLE(
    test_servicesystem ${TEST_SERVICESYSTEM_SRCS}
)

ADD_EXECUTABLE(
    test_marshalling ${TEST_MARSHALLING_SRCS}
)

########################################################

ADD_TEST( TestServicesystem     test_servicesystem )
ADD_TEST( TestMarshalling       test_marshalling )
