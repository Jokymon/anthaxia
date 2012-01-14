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
