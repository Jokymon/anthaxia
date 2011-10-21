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
