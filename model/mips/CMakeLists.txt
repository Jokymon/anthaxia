SET(MIPS_MODEL
    MipsModel.cpp
    MipsModel.h
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    plugins
    ${POCO_INCLUDE_PATH}
)

LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES( PocoFoundation PocoUtil )

ADD_LIBRARY( mips_processor SHARED ${MIPS_MODEL} )
IF(NOT UNIX) # Windows MS VC compiler specific
    SET_TARGET_PROPERTIES( mips_processor PROPERTIES PREFIX "" )
    SET_TARGET_PROPERTIES( mips_processor PROPERTIES LINK_FLAGS "-L${CMAKE_BINARY_DIR}" )
    TARGET_LINK_LIBRARIES( mips_processor procsim.dll )
    ADD_DEPENDENCIES(mips_processor procsim )
ENDIF()

SET_TARGET_PROPERTIES( mips_processor PROPERTIES 
    COMPILE_FLAGS "-I${CMAKE_SOURCE_DIR}/include -DBUILD_DLL"

    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

INSTALL(TARGETS
    mips_processor
    DESTINATION plugins
)


