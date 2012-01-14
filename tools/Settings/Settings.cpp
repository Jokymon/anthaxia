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

#include "Settings.h"
#include "Poco/AutoPtr.h"
#include "Poco/File.h"
#include "Poco/Util/PropertyFileConfiguration.h"

using Poco::Util::PropertyFileConfiguration;
using Poco::File;

namespace Settings {

    Poco::AutoPtr<PropertyFileConfiguration> pSettings;

    void loadSettings(const std::string& filename)
    {
        File f(filename);
        if (!f.exists())
        {
            f.createFile();
        }
        pSettings = new PropertyFileConfiguration(filename);
    }

    void saveSettings(const std::string& filename)
    {
        pSettings->save(filename);
    }

    bool hasKey(const std::string& key)
    {
        return pSettings->hasProperty(key);
    }

    void setString(const std::string& key, const std::string& value)
    {
        pSettings->setString(key, value);
    }

    std::string getString(const std::string& key, const std::string& default_value)
    {
        if (pSettings->hasProperty(key))
        {
            return pSettings->getString(key);
        } else
        {
            pSettings->setString(key, default_value);
            return default_value;
        }
    }

    void setInt(const std::string& key, int value)
    {
        pSettings->setInt(key, value);
    }

    int getInt(const std::string& key, int default_value)
    {
        if (pSettings->hasProperty(key))
        {
            return pSettings->getInt(key);
        } else
        {
            pSettings->setInt(key, default_value);
            return default_value;
        }
    }
}
