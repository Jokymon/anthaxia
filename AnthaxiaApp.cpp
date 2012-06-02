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

void AnthaxiaApp::initialize(Application& self) {
    Application::initialize(self);
    Settings::loadSettings("procsim.rc");

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
}
