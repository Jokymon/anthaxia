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

