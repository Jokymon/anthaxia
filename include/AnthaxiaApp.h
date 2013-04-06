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

#ifndef _ANTHAXIAAPP_H
#define _ANTHAXIAAPP_H

#include "Poco/Util/Application.h"
#include <vector>
#include <string>

class AnthaxiaApp : public Poco::Util::Application
{
public:
protected:
    void initialize(Application& self);
    void uninitialize();
    virtual int main(const std::vector<std::string>& args);

private:
};

#endif // _ANTHAXIAAPP_H
