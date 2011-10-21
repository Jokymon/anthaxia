#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <string>

namespace Settings
{
    void loadSettings(const std::string& filename);
    void saveSettings(const std::string& filename);

    bool hasKey(const std::string& key);
    
    void setString(const std::string& key, const std::string& value);
    std::string getString(const std::string& key, const std::string& default_value="");

    void setInt(const std::string& key, int value);
    int getInt(const std::string& key, int default_value=0);
}

#endif
