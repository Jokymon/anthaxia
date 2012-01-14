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

# Testing the servicesystem
SET(TEST_SERVICESYSTEM_SRCS
    TestServiceSystem.cpp
    AdderService.cpp
)

SET(TEST_MARSHALLING_SRCS
    TestMarshalling.cpp
)

INCLUDE_DIRECTORIES(
    ${POCO_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/packages/include
)

ADD_LIBRARY(gtest STATIC IMPORTED)
SET_TARGET_PROPERTIES(gtest PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/packages/lib/libgtest.a
)
ADD_LIBRARY(gtest_main STATIC IMPORTED)
SET_TARGET_PROPERTIES(gtest_main PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/packages/lib/libgtest_main.a
)

LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES(
    servicesystem
)

ADD_EXECUTABLE(
    test_servicesystem
    
    ${TEST_SERVICESYSTEM_SRCS}
    ${TEST_MARSHALLING_SRCS}
)
TARGET_LINK_LIBRARIES( test_servicesystem gtest_main )
TARGET_LINK_LIBRARIES( test_servicesystem gtest )

########################################################

ADD_TEST( TestServiceSystem        test_servicesystem )
