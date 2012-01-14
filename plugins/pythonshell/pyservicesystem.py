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

class ServiceProxy:
    def _createMethod(name):
        def _method(self, *args):
            return self._po.call_method(name, *args)
        return _method
    _createMethod = staticmethod(_createMethod)

    def __init__(self, name):
        import servicesystem
        from types import MethodType
        po = servicesystem.get_service_proxy(name)
        for (n, a, r) in po.get_method_list():
            if not hasattr(self, n):
                setattr(self, n, MethodType(ServiceProxy._createMethod(n), self, self.__class__))
        self._po = po

