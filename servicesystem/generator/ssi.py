import re, tpg

# TODO: need to add support for polymorph functions, functions with identical
# names but different parameter types
# TODO: Support for custom type parameters

class Class:
    def __init__(self, name):
        self.name = name
        self.functions = []

class Function:
    def __init__(self, name):
        self.name = name
        self.return_type = None
        self.parameter_types = []

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
                FUNCTION_DEF/f          $ s.functions.append( f )
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

    for fn in cls.functions:
        fn.signature = "".join( map(lambda x: x[1], fn.parameter_types) )

    nameSpace = {
        'classname' : cls.name,
        'functions' : cls.functions,
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
