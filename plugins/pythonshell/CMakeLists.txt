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
IF(MINGW)
    SET_TARGET_PROPERTIES( pythonshell PROPERTIES LINK_FLAGS "-Wl,--enable-auto-import" )
ENDIF()

IF(WIN32)
TARGET_LINK_LIBRARIES( pythonshell ${POCO_LIBRARY_PATH}/libPocoFoundation.dll )
ENDIF()

SET_TARGET_PROPERTIES( pythonshell PROPERTIES
    COMPILE_FLAGS "-DBUILD_DLL"

    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)



