ADD_SUBDIRECTORY(test)

SET(SERVICESYSTEM_SRCS
    marshalling.c
    marshalling.h
    servicesystem.cpp
    serviceprovider.h
    servicesystem.h

    ModelControlService.cpp
    ModelControlService.h
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

ADD_LIBRARY(
    servicesystem
    ${SERVICESYSTEM_SRCS}
)
