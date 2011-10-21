SET(SIMPLIFIED_MODEL
    SimplifiedModel.cpp
    SimplifiedModel.h
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    plugins
    ${POCO_INCLUDE_PATH}
)

LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES( PocoFoundation PocoUtil )

ADD_LIBRARY( simple_processor SHARED ${SIMPLIFIED_MODEL} )
IF(NOT UNIX) # Windows MS VC compiler specific
    SET_TARGET_PROPERTIES( simple_processor PROPERTIES PREFIX "" )
    SET_TARGET_PROPERTIES( simple_processor PROPERTIES LINK_FLAGS "-L${CMAKE_BINARY_DIR}" )
    TARGET_LINK_LIBRARIES( simple_processor procsim.dll )
    ADD_DEPENDENCIES(simple_processor procsim )
ENDIF()

SET_TARGET_PROPERTIES( simple_processor PROPERTIES 
    COMPILE_FLAGS "-I${CMAKE_SOURCE_DIR}/include -DBUILD_DLL"

    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

INSTALL(TARGETS
    simple_processor
    DESTINATION plugins
)


