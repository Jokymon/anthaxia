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

ADD_SUBDIRECTORY(mips)
ADD_SUBDIRECTORY(simplemodel)

SET(MODEL_SRCS
    ModelControl.cpp
    ModelControl.h
    SimulationObject.cpp
    SimulationObject.h
    SimulationObjectVisitor.cpp
    SystemOnChip.cpp
    SystemOnChip.h
    Processor.cpp
    Processor.h
    DummyRegister.cpp
    DummyRegister.h
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/tools
    ${POCO_INCLUDE_PATH}
)

LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES(
    PocoFoundation
    PocoUtil
)

ADD_LIBRARY( model ${MODEL_SRCS} )
