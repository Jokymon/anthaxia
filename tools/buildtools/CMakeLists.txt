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

SET(SRCS
    search_static_plugins.cpp
)

INCLUDE_DIRECTORIES(
    ${POCO_INCLUDE_PATH}
)
LINK_DIRECTORIES( ${POCO_LIBRARY_PATH} )
LINK_LIBRARIES( PocoFoundation PocoUtil )
ADD_EXECUTABLE( search_static_plugins
    ${SRCS}
)

SET_TARGET_PROPERTIES( search_static_plugins
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

