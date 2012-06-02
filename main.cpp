/***********************************************************************
 *
 * This file is part of Anthaxia.
 * 
 * Anthaxia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Anthaxia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Anthaxia.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "AnthaxiaApp.h"
#include "qt/PsUI.h"
#include "plugins/PluginManager.h"
#include "servicesystem/serviceprovider.h"
#include "servicesystem/ModelControlService.h"
#include "model/ModelControl.h"
#include "Logging/Logging.h"
#include "Settings/Settings.h"

#include "Poco/AutoPtr.h"
#include "Poco/Path.h"

//#ifdef _WIN32
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nShowCmd)
//#else
int main(int argc, char *argv[])
//#endif
{
	Poco::AutoPtr<AnthaxiaApp> theApp = new AnthaxiaApp;
    PsUI theUI;

//#ifdef _WIN32
//    theUI.parseArgs(__argc, __argv);
//#else
    theUI.parseArgs(argc, argv);
//#endif

    try {
        theApp->init(argc, argv);
    }
    catch (Poco::Exception& exc) {
    	theApp->logger().log(exc);
    }

    theApp->run();

    // TODO: parse arguments and were applicable change application settings

    PluginManager::init(theUI.getApplicationDirPath());
    // Make sure the model control is instantiated
    (void)ModelControl::getInstance();
    // Make sure the service system is present
    (void)ServiceSystem::getServiceSystem();
    registerModelControlService();

    theUI.install();
    theUI.loop();
    theUI.uninstall();

    Settings::saveSettings("procsim.rc");

    return 0;
}
