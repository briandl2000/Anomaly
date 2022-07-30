#include "Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"


namespace Anomaly
{
	struct InternalLogger
	{
		std::shared_ptr<spdlog::logger> s_CoreLogger;
		std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

	static InternalLogger internalLogger;

	bool InitializaLogging()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Anomaly.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		internalLogger.s_CoreLogger = std::make_shared<spdlog::logger>("Anomaly", begin(logSinks), end(logSinks));
		spdlog::register_logger(internalLogger.s_CoreLogger);
		internalLogger.s_CoreLogger->set_level(spdlog::level::trace);
		internalLogger.s_CoreLogger->flush_on(spdlog::level::trace);

		internalLogger.s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(internalLogger.s_ClientLogger);
		internalLogger.s_ClientLogger->set_level(spdlog::level::trace);
		internalLogger.s_ClientLogger->flush_on(spdlog::level::trace);

		return true;
	}

	void ShutdownLogging()
	{
	}

	void LogOutput(log_level level, const char* message, ...)
	{
		const char* level_strings[6] = {"[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };

		const uint32_t msg_length = 32000;
		char out_message[msg_length];
		memset(out_message, 0, sizeof(out_message));

		va_list arg_ptr;
		va_start(arg_ptr, message);
		vsnprintf(out_message, msg_length, message, arg_ptr);
		va_end(arg_ptr);

		char out_message2[msg_length];
		sprintf(out_message2, "%s%s", level_strings[level], out_message);

		switch (level)
		{
		case log_level::LOG_LEVEL_ERROR:
			internalLogger.s_ClientLogger->error(out_message2);
			break;
		case log_level::LOG_LEVEL_WARN:
			internalLogger.s_ClientLogger->warn(out_message2);
			break;
		case log_level::LOG_LEVEL_INFO:
			internalLogger.s_ClientLogger->info(out_message2);
			break;
		case log_level::LOG_LEVEL_DEBUG:
			internalLogger.s_ClientLogger->debug(out_message2);
			break;
		case log_level::LOG_LEVEL_TRACE:
			internalLogger.s_ClientLogger->trace(out_message2);
			break;
		}
	}

	void LogOutputCore(log_level level, const char* message, ...)
	{
		const char* level_strings[6] = {"[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };

		const uint32_t msg_length = 32000;
		char out_message[msg_length];
		memset(out_message, 0, sizeof(out_message));

		va_list arg_ptr;
		va_start(arg_ptr, message);
		vsnprintf(out_message, msg_length, message, arg_ptr);
		va_end(arg_ptr);

		char out_message2[msg_length];
		sprintf(out_message2, "%s%s", level_strings[level], out_message);

		switch (level)
		{
		case log_level::LOG_LEVEL_ERROR:
			internalLogger.s_CoreLogger->error(out_message2);
			break;
		case log_level::LOG_LEVEL_WARN:
			internalLogger.s_CoreLogger->warn(out_message2);
			break;
		case log_level::LOG_LEVEL_INFO:
			internalLogger.s_CoreLogger->info(out_message2);
			break;
		case log_level::LOG_LEVEL_DEBUG:
			internalLogger.s_CoreLogger->debug(out_message2);
			break;
		case log_level::LOG_LEVEL_TRACE:
			internalLogger.s_CoreLogger->trace(out_message2);
			break;
		}
	}
}
