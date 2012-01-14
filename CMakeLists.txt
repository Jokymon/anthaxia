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

###########################################################
# Needed modules

SET(POCO_INCLUDE_PATH "" CACHE PATH "Path to the Poco include files directory" )
SET(POCO_LIBRARY_PATH "" CACHE PATH "Path to the Poco library files directory" )

###########################################################
# Main
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(model)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(servicesystem)

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

##############################################################
# UI sources

SET(UI_SRCS
    qt/PsUI.cpp
    qt/PsMainWindow.cpp
    qt/PsMemoryObserver.cpp
    qt/PsMemView.cpp
    qt/PsModelControl.cpp
    qt/PsPluginInfoDlg.cpp
    qt/PsProcChooserDlg.cpp
    qt/PsRegisterView.cpp

    qt/PsUI.h
    qt/PsMainWindow.h
    qt/PsMemoryObserver.h
    qt/PsMemView.h
    qt/PsModelControl.h
    qt/PsPluginInfoDlg.h
    qt/PsProcChooserDlg.h
    qt/PsRegisterView.h

    qt/console/interceptor.cpp
    #qt/console/pymemoryproxy.cpp
    qt/console/qpluginconsole.cpp

    qt/console/interceptor.h
    #qt/console/pymemoryproxy.h
    qt/console/qpluginconsole.h

    ${CMAKE_BINARY_DIR}/PsUI.moc
    ${CMAKE_BINARY_DIR}/PsMainWindow.moc
    ${CMAKE_BINARY_DIR}/PsMemoryObserver.moc
    ${CMAKE_BINARY_DIR}/PsMemView.moc
    ${CMAKE_BINARY_DIR}/PsModelControl.moc
    ${CMAKE_BINARY_DIR}/PsRegisterView.moc
    ${CMAKE_BINARY_DIR}/PsPluginInfoDlg.moc
    ${CMAKE_BINARY_DIR}/PsProcChooserDlg.moc

    ${CMAKE_BINARY_DIR}/interceptor.moc
    ${CMAKE_BINARY_DIR}/qpluginconsole.moc

    ${CMAKE_BINARY_DIR}/static_plugins.h
)

##############################################################

INCLUDE(FindQt4)
IF (NOT QT4_FOUND)
	MESSAGE( FATAL_ERROR "QT4 is necessary")
ENDIF()
INCLUDE(UseQt4)
LINK_LIBRARIES(${QT_LIBRARIES})
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsUI.h              ${CMAKE_BINARY_DIR}/PsUI.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsMainWindow.h      ${CMAKE_BINARY_DIR}/PsMainWindow.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsMemoryObserver.h  ${CMAKE_BINARY_DIR}/PsMemoryObserver.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsMemView.h         ${CMAKE_BINARY_DIR}/PsMemView.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsModelControl.h    ${CMAKE_BINARY_DIR}/PsModelControl.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsRegisterView.h    ${CMAKE_BINARY_DIR}/PsRegisterView.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsPluginInfoDlg.h   ${CMAKE_BINARY_DIR}/PsPluginInfoDlg.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsProcChooserDlg.h  ${CMAKE_BINARY_DIR}/PsProcChooserDlg.moc)

QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/console/interceptor.h   ${CMAKE_BINARY_DIR}/interceptor.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/console/qpluginconsole.h      ${CMAKE_BINARY_DIR}/qpluginconsole.moc)

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
    servicesystem
)

ADD_EXECUTABLE( procsim 
    ${CORE_SRCS} ${ELFIO_SRCS} ${UI_SRCS}
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
