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
#
# Script to install all pre requisites for building the Processor simulator.
# Curently these are the Poco library and GMock. For running this script you
# need to have additional pre requisites installed, namely:
# - Python >= 2.4
# - pysvn package
# - CMake
# - some type of C/C++ compiler like GNU C, MinGW, Cygwin, Visual C++, ....
#
import os, os.path

config = {
    "poco_checkout_dir"  : "poco_checkout",
    "poco_required_rev"  : 1676,

    "gmock_download_url" : "http://googlemock.googlecode.com/files/gmock-1.6.0.zip",
    "gmock_unzip_dir"    : "gmock-1.6.0",
}

build_environment = {
    "MinGW" :
        { "cmake_generator" : "MinGW Makefiles", 
          "build_command"   : "mingw32-make",
          "lib_suffix"      : "dll",
          "lib_suffix2"     : "dll.a",
        },
    "Cygwin" :
        { "cmake_generator" : "Unix Makefiles",
          "build_command"   : "make",
          "lib_suffix"      : "so",
          "lib_suffix2"     : "so.13",
        },
    "Linux" :
        { "cmake_generator" : "Unix Makefiles",
          "build_command"   : "make",
          "lib_suffix"      : "so",
          "lib_suffix2"     : "so.13",
        },
}

def detect_build_environment():
    for path in os.environ["PATH"].split(os.pathsep):
        vc = os.path.join(path, "cl.exe")
        mingw = os.path.join(path, "mingw32-make.exe")
        cygwin = os.path.join(path, "make.exe")
        if os.path.exists( mingw ):
            return "MinGW"
        if os.path.exists( cygwin ):
            return "Cygwin"
        if os.name=="posix":
            return "Linux"
    return None

def find_cmake():
    # TODO: Either make sure CMake is 2.6.2 (as required by GTest) or even 2.8 as required by Poco or patch the CMakeLists files
    import os
    paths = []
    if os.name=='nt':
        paths = [
            os.path.join( os.environ["ProgramFiles"], "CMake 2.8", "bin", "cmake.exe" ),
            os.path.join( os.environ["ProgramFiles"], "CMake 2.6", "bin", "cmake.exe" ),
        ]
    else:
        paths = [
            "/usr/bin/cmake",
        ]
    for p in paths:
        if os.path.exists(p):
            return p
    raise IOError("Couldn't find cmake executable")

def copy_tree(src_dir, dst_dir, action=lambda src, tgt: None, filt=lambda file_name: True):
    import os.path, shutil
    from distutils.dir_util import mkpath
    if not os.path.isdir(src_dir) or not os.path.isdir(dst_dir):
        raise IOError("Source and Destination must be directories")
    for root, dirs, files in os.walk(src_dir):
        common_prefix = os.path.commonprefix([src_dir, root])
        target_path = os.path.join( dst_dir, root[len(common_prefix)+1:] )
        if filt( target_path ) and not os.path.exists( target_path ):
            mkpath( target_path )
        for f in files:
            if filt( os.path.join(root, f) ):
                src = os.path.join( root, f )
                tgt = os.path.join( target_path, f )
                if action:
                    action(src, tgt)
                shutil.copy2( src, tgt )

# -----------------------------------------------------------------------------

def install_poco(build_env):
    import pysvn, subprocess
    def notify( event_dict ):
        print(event_dict["path"])
    s = pysvn.Client()
    s.callback_notify = notify

    print("Checkout Poco...")
    s.checkout( "http://poco.svn.sourceforge.net/svnroot/poco/poco/trunk/", config["poco_checkout_dir"], revision=pysvn.Revision( pysvn.opt_revision_kind.number, 1676 ) )

    print("Compiling Poco...")
    cmake_executable = find_cmake()
    current_dir = os.getcwd()
    os.chdir( config["poco_checkout_dir"] )
    if not os.path.exists( "build_dir" ):
        os.mkdir( "build_dir" )
    os.chdir( "build_dir" )
    # TODO: Add error handling; no need to continue here if the build fails
    subprocess.call(["%s" % cmake_executable, "-G", build_environment[build_env]["cmake_generator"], ".."])
    os.system( build_environment[build_env]["build_command"] )

    print("Installing Poco...")
    from distutils.dir_util import mkpath
    import shutil
    if not os.path.exists("../../packages/lib"):
        mkpath("../../packages/lib")
    if not os.path.exists("../../packages/include"):
        mkpath("../../packages/include")

    print("  installing Foundation")
    copy_tree("../Foundation/include", "../../packages/include", 
        filt = lambda f: ".svn" not in f)
    shutil.copy2("Foundation/libPocoFoundation." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("Foundation/libPocoFoundation." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")
    shutil.copy2("Foundation/libPocoFoundationd." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("Foundation/libPocoFoundationd." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")

    print("  installing Data")
    copy_tree("../Data/include", "../../packages/include", 
        filt = lambda f: ".svn" not in f)
    shutil.copy2("Data/libPocoData." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("Data/libPocoData." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")
    shutil.copy2("Data/libPocoDatad." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("Data/libPocoDatad." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")

    print("  installing Util")
    copy_tree("../Util/include", "../../packages/include", 
        filt = lambda f: ".svn" not in f)
    shutil.copy2("Util/libPocoUtil." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("Util/libPocoUtil." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")
    shutil.copy2("Util/libPocoUtild." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("Util/libPocoUtild." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")

    print("  installing Net")
    copy_tree("../Net/include", "../../packages/include", 
        filt = lambda f: ".svn" not in f)
    shutil.copy2("Net/libPocoNet." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("Net/libPocoNet." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")
    shutil.copy2("Net/libPocoNetd." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("Net/libPocoNetd." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")

    print("  installing XML")
    copy_tree("../XML/include", "../../packages/include", 
        filt = lambda f: ".svn" not in f)
    shutil.copy2("XML/libPocoXML." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("XML/libPocoXML." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")
    shutil.copy2("XML/libPocoXMLd." + build_environment[build_env]["lib_suffix"], "../../packages/lib")
    shutil.copy2("XML/libPocoXMLd." + build_environment[build_env]["lib_suffix2"], "../../packages/lib")

    os.chdir( current_dir )

def install_gmock(build_env):
    import urllib, zipfile, subprocess, sys
    if sys.version_info[0]==2:
        from urllib import urlretrieve
    elif sys.version_info[0]==3:
        from urllib.request import urlretrieve
    else:
        print("ERROR: Don't know where to get function 'urlretrieve' from. Stopping installation of GMock")
        return

    print("Downloading GMock...")
    urlretrieve( config["gmock_download_url"], "gmock.zip" )
    zf = zipfile.ZipFile( "gmock.zip", "r" )
    for n in zf.namelist():
        print(n)
        # TODO: Implement a more Python 2.5 friendly version
        zf.extract( n )

    print("Compiling GMock...")
    cmake_executable = find_cmake()
    current_dir = os.getcwd()
    os.chdir( config["gmock_unzip_dir"] )
    if not os.path.exists( "build_dir" ):
        os.mkdir( "build_dir" )
    os.chdir( "build_dir" )
    subprocess.call(
        ["%s" % cmake_executable, 
         "-G", build_environment[build_env]["cmake_generator"], 
         "-Dgtest_disable_pthreads=1",
         ".."])
    os.system( build_environment[build_env]["build_command"] )

    print("Installing GMock...")
    from distutils.dir_util import mkpath
    import shutil
    if not os.path.exists("../../packages/lib"):
        mkpath("../../packages/lib")
    if not os.path.exists("../../packages/include"):
        mkpath("../../packages/include")

    copy_tree("../include", "../../packages/include")
    shutil.copy2("libgmock.a", "../../packages/lib")
    shutil.copy2("libgmock_main.a", "../../packages/lib")
    copy_tree("../gtest/include", "../../packages/include")
    shutil.copy2("gtest/libgtest.a", "../../packages/lib")
    shutil.copy2("gtest/libgtest_main.a", "../../packages/lib")

    os.chdir( current_dir )

if __name__=="__main__":
    build_env = detect_build_environment()
    install_poco(build_env)
    install_gmock(build_env)
