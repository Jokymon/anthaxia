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

# Header
PROJECT(ProcSim)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
CMAKE_POLICY(SET CMP0003 OLD)
ENABLE_TESTING()
# VERSION string stored in config.h
SET(VERSION "\"0.0.1\"")
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

# fix for X86 64 bit version
IF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC"  )
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -fPIC"  )
ENDIF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")

###########################################################
# Needed modules

SET(POCO_INCLUDE_PATH "" CACHE PATH "Path to the Poco include files directory" )
SET(POCO_LIBRARY_PATH "" CACHE PATH "Path to the Poco library files directory" )

# TODO: This is a bit annoying, I would like to refactor PsUI so that the main
# application doesn't need to include Qt headers
INCLUDE(FindQt4)
IF (NOT QT4_FOUND)
	MESSAGE( FATAL_ERROR "QT4 is necessary")
ENDIF()
INCLUDE(UseQt4)

###########################################################
# Main
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(model)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(servicesystem)
ADD_SUBDIRECTORY(qt)

SET(CORE_SRCS_WINDOWS
    platform/windows/DynamicLibrary.cpp
)

SET(CORE_SRCS_LINUX
    platform/linux/DynamicLibrary.cpp
)

SET(CORE_SRCS
    main.cpp
    model/ModelControl.cpp
    model/ModelControl.h
    platform/DynamicLibrary.h

    plugins/PluginManager.cpp
    plugins/PluginManager.h

    plugins/PsBinLoader.cpp
    plugins/PsBinLoader.h

    tools/Logging/Logging.h
    tools/Settings/Settings.cpp
    tools/Settings/Settings.h

    include/plugins/Plugin.h
    include/plugins/processor/MemoryInterface.h
    include/plugins/processor/ProcessorControl.h

    ${CMAKE_BINARY_DIR}/static_plugins.h
)

IF (UNIX)
    SET (CORE_SRCS ${CORE_SRCS} ${CORE_SRCS_LINUX} )
ELSEIF(WIN32)
    SET (CORE_SRCS ${CORE_SRCS} ${CORE_SRCS_WINDOWS} )
ENDIF()

SET(ELFIO_SRCS
    plugins/elfloader/PsElfLoader.cpp
    plugins/elfloader/ELFIO.cpp
    plugins/elfloader/ELFIDynamic.cpp
    plugins/elfloader/ELFIImpl.cpp
    plugins/elfloader/ELFINote.cpp
    plugins/elfloader/ELFIOUtils.cpp
    plugins/elfloader/ELFIRelocation.cpp
    plugins/elfloader/ELFISection.cpp
    plugins/elfloader/ELFISegment.cpp
    plugins/elfloader/ELFIStrings.cpp
    plugins/elfloader/ELFISymbols.cpp
    plugins/elfloader/ELFODynamic.cpp
    plugins/elfloader/ELFOImpl.cpp
    plugins/elfloader/ELFONote.cpp
    plugins/elfloader/ELFORelocation.cpp
    plugins/elfloader/ELFOSection.cpp
    plugins/elfloader/ELFOSegment.cpp
    plugins/elfloader/ELFOString.cpp
    plugins/elfloader/ELFOSymbols.cpp

    plugins/elfloader/PsElfLoader.h
    plugins/elfloader/ELFIO.h
    plugins/elfloader/ELFI.h
    plugins/elfloader/ELFIImpl.h
    plugins/elfloader/ELFIOUtils.h
    plugins/elfloader/ELFO.h
    plugins/elfloader/ELFOImpl.h
    plugins/elfloader/ELFTypes.h
)

#################################################
# Instructions for building the main application

IF(NOT UNIX AND NOT MINGW) # Windows MS VC compiler specific
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF()

INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
    include
    tools
    plugins
    qt
    ${POCO_INCLUDE_PATH}
)

LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES( 
    PocoFoundation
    PocoUtil
    gui
    servicesystem
)

ADD_EXECUTABLE( procsim 
    ${CORE_SRCS} ${ELFIO_SRCS}
)

ADD_CUSTOM_COMMAND(
    OUTPUT static_plugins.h
    COMMAND search_static_plugins --base-path=${CMAKE_SOURCE_DIR}/plugins --output-file=static_plugins.h
    DEPENDS search_static_plugins
    VERBATIM
)

SET_TARGET_PROPERTIES( procsim PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY bin
)
IF(NOT UNIX)
    SET( PROCSIM_LINKER_FLAGS "-Wl,--export-all-symbols -Wl,--out-implib,libprocsim.dll.a" )
#    IF (MINGW)
#        SET( PROCSIM_LINKER_FLAGS "${PROCSIM_LINKER_FLAGS} -mwindows")
#    ENDIF()
    SET_TARGET_PROPERTIES( procsim PROPERTIES LINK_FLAGS ${PROCSIM_LINKER_FLAGS} )
ENDIF()

########################################################################################
# Packaging information
########################################################################################

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "1")

INSTALL(TARGETS procsim
    DESTINATION bin
)

INSTALL(FILES
    logging.cfg
    DESTINATION bin
)

INSTALL(FILES
    ${CMAKE_SOURCE_DIR}/icons/16-file-page.png
    ${CMAKE_SOURCE_DIR}/icons/Document.png
    ${CMAKE_SOURCE_DIR}/icons/Play.png
    ${CMAKE_SOURCE_DIR}/icons/Reset.png
    ${CMAKE_SOURCE_DIR}/icons/Step.png
    ${CMAKE_SOURCE_DIR}/icons/Stop.png
    ${CMAKE_SOURCE_DIR}/icons/Terminal.png
    DESTINATION icons
)

GET_FILENAME_COMPONENT(${CMAKE_C_COMPILER} MINGW_BINARY_PATH PATH ABSOLUTE CACHE )
IF (NOT UNIX)
    INSTALL(FILES
        ${MINGW_BINARY_PATH}/mingwm10.dll
        ${QT_BINARY_DIR}/QtCore4.dll
        ${QT_BINARY_DIR}/QtGui4.dll
        ${QT_BINARY_DIR}/QtXml4.dll
        ${QT_BINARY_DIR}/QtNetwork4.dll
        ${POCO_LIBRARY_PATH}/libPocoFoundation.dll
        ${POCO_LIBRARY_PATH}/libPocoUtil.dll
        ${POCO_LIBRARY_PATH}/libPocoXML.dll
    DESTINATION bin
    )
ENDIF()

SET(CPACK_PACKAGE_EXECUTABLES "procsim" "Silvan's Pluggable Processor Simulator")
INCLUDE(CPack)
