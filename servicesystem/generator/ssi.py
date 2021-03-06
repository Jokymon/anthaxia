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

import re, tpg

# TODO: need to add support for polymorph functions, functions with identical
# names but different parameter types
# TODO: Support for custom type parameters

class Class:
    def __init__(self, name):
        self.name = name
        self.functions = {}

    def add_function(self, func):
        if not func.name in self.functions.keys():
            self.functions[ func.name ] = {}
        self.functions[ func.name ][ func.signature() ] = func

    def all_functions(self):
        for fns in self.functions.values():
            for fn in fns.values():
                yield fn

class Function:
    def __init__(self, name):
        self.name = name
        self.return_type = None
        self.parameter_types = []

    def signature(self):
        return "".join( map(lambda x: x[1], self.parameter_types) )

class UnknownTypeException(Exception):
    def __init__(self, t):
        self.type_name = t
    def __str__(self):
        return "Unknown type %s" % self.type_name

class SSIParser(tpg.Parser):
    r"""
        separator space     '\s+';
        separator comment   '#.*';

        token identifier    '[a-zA-Z][a-zA-Z0-9_]*';

        START/s ->
            'class' identifier/i        $ s = Class(i)
            (
                FUNCTION_DEF/f          $ s.add_function( f )
            )*
            'end'
            ;

        FUNCTION_DEF/fn ->
            identifier/fname            $ fn = Function(fname)
            '\('
            ( TYPE/param                $ fn.parameter_types.append(param)
                ( ',' TYPE/param        $ fn.parameter_types.append(param)
                )*
            )?
            '\)'
            ':' TYPE/return_type        $ fn.return_type = return_type
            ;

        TYPE/t ->
            identifier/i                $ if not i in self.symboltable.keys():
                                        $     raise UnknownTypeException(i)
                                        $ else:
                                        $     t = self.symboltable[i]
            ;
    """

    def __init__(self):
        tpg.Parser.__init__(self)

        self.symboltable = {
            #         C/C++ type, type signature
            'void'   : ( 'void', '' ),
            'int'    : ( 'int', 'i' ),
            'string' : ( 'char*', 's' ),
        }

def generate_cheetah(cls):
    from Cheetah.Template import Template

    inp = open("classfile.cheetah", "r")
    src_template = inp.readlines()
    src_template = "".join(src_template)
    inp.close()

    nameSpace = {
        'classname' : cls.name,
        'functions' : cls.all_functions,
    }

    templ = Template( src_template, searchList = [nameSpace] )
    oup = open("%s.cpp" % cls.name, "w")
    oup.write( str(templ) )
    oup.close()

if __name__=="__main__":
    from optparse import OptionParser

    parser = OptionParser()
    (option, args) = parser.parse_args()

    inputfilename = args[0]
    input         = open(inputfilename, "r")
    inputcontent  = input.readlines()
    input.close()

    inputcontent = "".join(inputcontent)
    ssiParser = SSIParser()
    result = ssiParser( inputcontent )
    generate_cheetah( result )
