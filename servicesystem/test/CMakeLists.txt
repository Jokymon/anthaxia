# Testing the servicesystem
SET(TEST_SERVICESYSTEM_SRCS
    TestServiceSystem.cpp
    AdderService.cpp
)

SET(TEST_MARSHALLING_SRCS
    TestMarshalling.cpp
)

INCLUDE_DIRECTORIES(
    ${POCO_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/packages/include
)

ADD_LIBRARY(gtest STATIC IMPORTED)
SET_TARGET_PROPERTIES(gtest PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/packages/lib/libgtest.a
)
ADD_LIBRARY(gtest_main STATIC IMPORTED)
SET_TARGET_PROPERTIES(gtest_main PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/packages/lib/libgtest_main.a
)

LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES(
    servicesystem
)

ADD_EXECUTABLE(
    test_servicesystem
    
    ${TEST_SERVICESYSTEM_SRCS}
    ${TEST_MARSHALLING_SRCS}
)
TARGET_LINK_LIBRARIES( test_servicesystem gtest_main )
TARGET_LINK_LIBRARIES( test_servicesystem gtest )

########################################################

ADD_TEST( TestServiceSystem        test_servicesystem )
