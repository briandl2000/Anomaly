#pragma once

#include "EngineDefines.h"

#include "DataStructures.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

// Disable debug and trace logging for release builds.
#ifdef ARELEASE
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

namespace Anomaly::logger
{
    enum eLogLevel
    {
        LOG_LEVEL_FATAL = 0,
        LOG_LEVEL_ERROR = 1,
        LOG_LEVEL_WARN = 2,
        LOG_LEVEL_INFO = 3,
        LOG_LEVEL_DEBUG = 4,
        LOG_LEVEL_TRACE = 5
    };

    bool Initialize();
    void Shutdown();
    ANOM_API void LogOutput(eLogLevel level, AString message, ...);

}

#ifndef AFATAL
#define AFATAL(message, ...) Anomaly::logger::LogOutput(Anomaly::logger::eLogLevel::LOG_LEVEL_FATAL, message, ##__VA__ARGS__);
#endif


#ifndef AERROR
// Logs an error-level message.
#define AERROR(message, ...) Anomaly::logger::LogOutput(Anomaly::logger::eLogLevel::LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
// Logs a warning-level message.
#define AWARN(message, ...) Anomaly::logger::LogOutput(Anomaly::logger::eLogLevel::LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_WARN_ENABLED != 1
#define AWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
// Logs a info-level message.
#define AINFO(message, ...) Anomaly::logger::LogOutput(Anomaly::logger::eLogLevel::LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_INFO_ENABLED != 1
#define AINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
// Logs a debug-level message.
#define ADEBUG(message, ...) Anomaly::logger::LogOutput(Anomaly::logger::eLogLevel::LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_DEBUG_ENABLED != 1
#define ADEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
// Logs a trace-level message.
#define ATRACE(message, ...) Anomaly::logger::LogOutput(Anomaly::logger::eLogLevel::LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_TRACE_ENABLED != 1
#define ATRACE(message, ...)
#endif