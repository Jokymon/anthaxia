#include "qt/PsUI.h"
#include "plugins/PluginManager.h"
#include "servicesystem/serviceprovider.h"
#include "servicesystem/ModelControlService.h"
#include "model/ModelControl.h"
#include "Logging/Logging.h"
#include "Settings/Settings.h"

#include "Poco/AutoPtr.h"
#include "Poco/Path.h"
#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/Util/PropertyFileConfiguration.h"

using Poco::Path;
using Poco::Util::LoggingConfigurator;
using Poco::Util::AbstractConfiguration;
using Poco::Util::MapConfiguration;
using Poco::Util::PropertyFileConfiguration;

//#ifdef _WIN32
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nShowCmd)
//#else
int main(int argc, char *argv[])
//#endif
{
    PsUI theUI;

    Settings::loadSettings("procsim.rc");

//#ifdef _WIN32
//    theUI.parseArgs(__argc, __argv);
//#else
    theUI.parseArgs(argc, argv);
//#endif

    // TODO: parse arguments and were applicable change application settings

	Poco::AutoPtr<AbstractConfiguration> pConfig;
    if (Path("logging.cfg").isFile())
    {
        pConfig = new PropertyFileConfiguration("logging.cfg");
    } else
    {
        pConfig = new MapConfiguration();
    }
	LoggingConfigurator configurator;
	configurator.configure(pConfig);

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
