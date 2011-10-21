#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/RegularExpression.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionProcessor.h"
#include "Poco/Util/HelpFormatter.h"
#include <fstream>
#include <iostream>
#include <string>

using Poco::Glob;
using Poco::File;
using Poco::Path;
using Poco::RegularExpression;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionProcessor;
using Poco::Util::HelpFormatter;

void grep(File& base, RegularExpression& regex, std::set<std::string>& findings)
{
    std::vector<File> files;
    base.list(files);
    for (std::vector<File>::iterator iter = files.begin();
         iter != files.end();
         ++iter)
    {
        if (iter->isFile())
        {
            std::ifstream grep_file(iter->path().c_str());
            std::string buffer;
            while (!grep_file.eof())
            {
                std::getline(grep_file, buffer);
                RegularExpression::MatchVec matches;
                if (regex.match(buffer, 0, matches)==2)
                {
                    findings.insert(buffer.substr(matches[1].offset, matches[1].length));
                }
            }
            grep_file.close();
        } else if (iter->isDirectory())
        {
            grep(*iter, regex, findings);
        }
    }
}

void displayHelp(OptionSet& set)
{
    HelpFormatter formatter(set);
    formatter.setCommand("search_static_plugins");
    formatter.setUsage("OPTIONS");
    formatter.setHeader("Searches all files below the given base path for the pattern\n\n"
        "   STATIC_PLUGIN_INIT void <initfunction>(...);\n\n"
        "and stores them in the specified output file as external declarations and "
        "calls to such functions.");
    formatter.format(std::cout);
}

int main(int argc, char* argv[])
{
    OptionSet set;
    set.addOption(
        Option("base-path", "P", "specify a base path to start searching")
            .required(true)
            .argument("path", true));

    set.addOption(
        Option("output-file", "o", "specify the output file")
            .required(true)
            .argument("file", true));

    OptionProcessor proc(set);
    
    std::string base_path, output_file;
    try
    {
        std::string name;
        std::string value;
        for (int i=0; i<argc; i++)
        {
            proc.process(argv[i], name, value);
            if (name=="base-path")
                base_path = value;
            else if (name=="output-file")
                output_file = value;
        }
        proc.checkRequired();
    }
    catch (...)
    {
        displayHelp(set);
        return 1;
    }

    // Find all files below the base-path that contain the given pattern
    std::set<std::string> files;
    //RegularExpression re("$STATIC_PLUGIN_INIT[ \t]+void[ \t]+register([a-zA-Z_][a-zA-Z0-9_]*)\\(PluginManager[ \t]*\\*[ \t]*[a-zA-Z_][a-zA-Z0-9_]+\\)");
    RegularExpression re("STATIC_PLUGIN_INIT[ \t]+void[ \t]+(register[a-zA-Z_][a-zA-Z0-9_]*)\\(PluginManager[ \t]*\\*[ \t]*[a-zA-Z_][a-zA-Z0-9_]+\\)");
    File base_file(base_path);
    grep(base_file, re, files);

    std::ofstream plugin_header(output_file.c_str());

    plugin_header << "#ifndef _PLUGIN_HEADER_H" << std::endl;
    plugin_header << "#define _PLUGIN_HEADER_H" << std::endl << std::endl;
    plugin_header << "class PluginManager;" << std::endl << std::endl;

    // add external declarations of all plugin init functions
    plugin_header << "extern \"C\" {" << std::endl;
    for (std::set<std::string>::iterator iter = files.begin();
         iter != files.end();
         ++iter)
    {
        plugin_header << "    extern void " << *iter << "(PluginManager* pm);" << std::endl;
    }
    plugin_header << "}" << std::endl;

    // create macro for calling all plugin initializer functions
    plugin_header << std::endl << "#define LOAD_STATIC_PLUGINS(manager) \\" << std::endl;
    for (std::set<std::string>::iterator iter = files.begin();
         iter != files.end();
         ++iter)
    {
        plugin_header << "\t" << *iter << "(manager); \\" << std::endl;
    }

    plugin_header << std::endl << "#endif" << std::endl;

    plugin_header.close();

    return 0;
}
