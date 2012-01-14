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

#ifndef _EXTENDED_LOGGING_H
#define _EXTENDED_LOGGING_H

#include "Poco/Logger.h"
#include "Poco/LogStream.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Path.h"

#define DEFINE_STATIC_LOGGER(hierarchy) \
    namespace { \
    Poco::LogStream log_stream(Poco::Logger::get(hierarchy)); \
    }

#define LOG_DEBUG(message) \
    log_stream.debug() << Poco::Path(__FILE__).getFileName() << ":" << __LINE__ << " - " << message << std::endl
#define LOG_WARN(message) \
    log_stream.warning() << Poco::Path(__FILE__).getFileName() << ":" << __LINE__ << " - " << message << std::endl
#define LOG_ERROR(message) \
    log_stream.error() << Poco::Path(__FILE__).getFileName() << ":" << __LINE__ << " - " << message << std::endl

#endif
