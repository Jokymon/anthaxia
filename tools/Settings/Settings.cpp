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
