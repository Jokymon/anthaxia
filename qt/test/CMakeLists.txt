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

FIND_PACKAGE(Qt4 4.6.3 REQUIRED QtCore QtGui QtTest)
INCLUDE(${QT_USE_FILE})

SET(UI_TESTING_SRCS
    TestSimulationModel.cpp

    main.cpp

    ${CMAKE_BINARY_DIR}/qt/test/TestSimulationModel.moc
)

QT4_GENERATE_MOC(TestSimulationModel.h   ${CMAKE_BINARY_DIR}/qt/test/TestSimulationModel.moc)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/qt
    ${CMAKE_BINARY_DIR}/qt/test
)

LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES(gui model ${QT_LIBRARIES})

ADD_EXECUTABLE(test_ui
    ${UI_TESTING_SRCS}
)

########################################################

ADD_TEST( TestUi    test_ui )
