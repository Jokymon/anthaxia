########################################################################
#
# This file is part of Anthaxia.
# 
# Anthaxia is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Anthaxia is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Anthaxia.  If not, see <http://www.gnu.org/licenses/>.
#
########################################################################

SET(SIMPLIFIED_MODEL
    SimplifiedModel.cpp
    SimplifiedModel.h
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    plugins
    ${POCO_INCLUDE_PATH}
)

LINK_DIRECTORIES( ${CMAKE_BINARY_DIR} ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES( PocoFoundation PocoUtil )

ADD_LIBRARY( simple_processor SHARED ${SIMPLIFIED_MODEL} )
IF(NOT UNIX) # Windows MS VC compiler specific
    SET_TARGET_PROPERTIES( simple_processor PROPERTIES PREFIX "" )
    TARGET_LINK_LIBRARIES( simple_processor procsim.dll )
    ADD_DEPENDENCIES(simple_processor procsim )
ENDIF()

SET_TARGET_PROPERTIES( simple_processor PROPERTIES 
    COMPILE_FLAGS "-DBUILD_DLL"

    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

INSTALL(TARGETS
    simple_processor
    DESTINATION plugins
)


