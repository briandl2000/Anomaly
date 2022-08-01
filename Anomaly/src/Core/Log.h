 #pragma once

#include "EngineDefines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if ANOM_RELEASE | ANOM_DIST
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

namespace Anomaly
{
	enum log_level
	{
		LOG_LEVEL_ERROR = 0,
		LOG_LEVEL_WARN = 1,
		LOG_LEVEL_INFO = 2,
		LOG_LEVEL_DEBUG = 3,
		LOG_LEVEL_TRACE = 4
	};

	ANOM_API bool InitializaLogging();
	ANOM_API void ShutdownLogging();

	ANOM_API void LogOutput(log_level level, const char* message, ...);
	void LogOutputCore(log_level level, const char* message, ...);
}

#ifndef ANOM_ERROR
#define ANOM_ERROR(message, ...) LogOutput(Anomaly::log_level::LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#define ANOM_CORE_ERROR(message, ...) LogOutputCore(Anomaly::log_level::LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#define ANOM_WARN(message, ...) LogOutput(Anomaly::log_level::LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#define ANOM_CORE_WARN(message, ...) LogOutputCore(Anomaly::log_level::LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define ANOM_WARN(message, ...)
#define ANOM_CORE_WARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define ANOM_INFO(message, ...) LogOutput(Anomaly::log_level::LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#define ANOM_CORE_INFO(message, ...) LogOutputCore(Anomaly::log_level::LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define ANOM_INFO(message, ...)
#define ANOM_CORE_INFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define ANOM_DEBUG(message, ...) LogOutput(Anomaly::log_level::LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#define ANOM_CORE_DEBUG(message, ...) LogOutputCore(Anomaly::log_level::LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define ANOM_DEBUG(message, ...)
#define ANOM_CORE_DEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define ANOM_TRACE(message, ...) LogOutput(Anomaly::log_level::LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#define ANOM_CORE_TRACE(message, ...) LogOutputCore(Anomaly::log_level::LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define ANOM_TRACE(message, ...)
#define ANOM_CORE_TRACE(message, ...)
#endif
