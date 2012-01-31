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

from optparse import OptionParser

header_file_template="""#ifndef {include_guard}
#define {include_guard}

class {class_name}
{{
public:

private:
}};

#endif // {include_guard}
"""

source_file_template="""#include "{include_file}"
"""

def ask_with_default(question, default):
    user_in = raw_input("%s [%s]: " % (question, default))
    if user_in.strip() == "":
        user_in = default
    return user_in

def interactive_query_options(options):
    classname = ask_with_default("Class name", options.classname or "")
    dest_path = ask_with_default("Destination path", options.destpath or "")
    # TODO: do some input validation
    options.classname = classname
    options.destpath = dest_path

def main():
    parser = OptionParser()
    parser.add_option("-c", "--classname", dest="classname",
                      help="Name of the class to generate")
    parser.add_option("-p", "--path", dest="destpath",
                      help="Destination path to generate the class to")
    (options, args) = parser.parse_args()

    if options.classname==None or options.destpath==None:
        interactive_query_options(options)

    import os.path
    header_file = options.classname + ".h"
    header_file_path = os.path.join(options.destpath, header_file)
    source_file = options.classname + ".cpp"
    source_file_path = os.path.join(options.destpath, source_file)

    definitions = {
        "include_file"  : header_file,
        "include_guard" : "_%s" % header_file.upper().replace(".", "_"),
        "class_name"    : options.classname,
    }

    f = open( header_file_path, "w" )
    f.write( header_file_template.format( **definitions ) )
    f.close()

    f = open( source_file_path, "w" )
    f.write( source_file_template.format( **definitions ) )
    f.close()

if __name__=="__main__":
    main()
