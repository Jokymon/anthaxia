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

ADD_SUBDIRECTORY(test)

INCLUDE(FindQt4)
IF (NOT QT4_FOUND)
	MESSAGE( FATAL_ERROR "QT4 is necessary")
ENDIF()
INCLUDE(UseQt4)

SET(UI_SRCS
    PsUI.cpp
    PsMainWindow.cpp
    PsMemoryObserver.cpp
    PsMemView.cpp
    PsModelControl.cpp
    PsPluginInfoDlg.cpp
    PsProcChooserDlg.cpp
    PsRegisterView.cpp
    PsSimulationModel.cpp

    console/interceptor.cpp
    #console/pymemoryproxy.cpp
    console/qpluginconsole.cpp

    ${CMAKE_BINARY_DIR}/qt/PsUI.moc
    ${CMAKE_BINARY_DIR}/qt/PsMainWindow.moc
    ${CMAKE_BINARY_DIR}/qt/PsMemoryObserver.moc
    ${CMAKE_BINARY_DIR}/qt/PsMemView.moc
    ${CMAKE_BINARY_DIR}/qt/PsModelControl.moc
    ${CMAKE_BINARY_DIR}/qt/PsRegisterView.moc
    ${CMAKE_BINARY_DIR}/qt/PsPluginInfoDlg.moc
    ${CMAKE_BINARY_DIR}/qt/PsProcChooserDlg.moc
    ${CMAKE_BINARY_DIR}/qt/PsSimulationModel.moc

    ${CMAKE_BINARY_DIR}/qt/interceptor.moc
    ${CMAKE_BINARY_DIR}/qt/qpluginconsole.moc
)

QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsUI.h              ${CMAKE_BINARY_DIR}/qt/PsUI.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsMainWindow.h      ${CMAKE_BINARY_DIR}/qt/PsMainWindow.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsMemoryObserver.h  ${CMAKE_BINARY_DIR}/qt/PsMemoryObserver.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsMemView.h         ${CMAKE_BINARY_DIR}/qt/PsMemView.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsModelControl.h    ${CMAKE_BINARY_DIR}/qt/PsModelControl.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsRegisterView.h    ${CMAKE_BINARY_DIR}/qt/PsRegisterView.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsPluginInfoDlg.h   ${CMAKE_BINARY_DIR}/qt/PsPluginInfoDlg.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsProcChooserDlg.h  ${CMAKE_BINARY_DIR}/qt/PsProcChooserDlg.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/PsSimulationModel.h ${CMAKE_BINARY_DIR}/qt/PsSimulationModel.moc)

QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/console/interceptor.h   ${CMAKE_BINARY_DIR}/qt/interceptor.moc)
QT4_GENERATE_MOC(${CMAKE_SOURCE_DIR}/qt/console/qpluginconsole.h      ${CMAKE_BINARY_DIR}/qt/qpluginconsole.moc)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    
    # TODO: this feels ugly, maybe we should move those headers somewhere else
    ${CMAKE_SOURCE_DIR}/tools
    ${CMAKE_SOURCE_DIR}/plugins

    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/qt
    ${POCO_INCLUDE_PATH}
)

LINK_LIBRARIES(${QT_LIBRARIES})

ADD_LIBRARY( gui ${UI_SRCS} )

