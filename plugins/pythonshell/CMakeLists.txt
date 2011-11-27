INCLUDE(FindPythonLibs)

SET(PYTHON_SHELL
    PythonShell.cpp
    PythonShell.h
    pyservicesystem.c
    pyservicesystem.h
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${POCO_INCLUDE_PATH}
    ${PYTHON_INCLUDE_PATH}
)

LINK_DIRECTORIES( ${CMAKE_BINARY_DIR} )
LINK_LIBRARIES(
    ${PYTHON_LIBRARIES}
)

ADD_LIBRARY( pythonshell SHARED ${PYTHON_SHELL} )
IF(NOT UNIX) # Windows MS VC compiler specific
    SET_TARGET_PROPERTIES( pythonshell PROPERTIES PREFIX "" )
    TARGET_LINK_LIBRARIES( pythonshell procsim.dll )
    ADD_DEPENDENCIES(pythonshell procsim )
ENDIF()

IF(WIN32)
TARGET_LINK_LIBRARIES( pythonshell ${POCO_LIBRARY_PATH}/libPocoFoundation.dll )
ENDIF()

SET_TARGET_PROPERTIES( pythonshell PROPERTIES
    COMPILE_FLAGS "-DBUILD_DLL"

    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)



