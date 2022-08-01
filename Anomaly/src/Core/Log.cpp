#include "anompch.h"
#include "Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"


namespace Anomaly
{
	struct LoggerState
	{
		std::shared_ptr<spdlog::logger> CoreLogger;
		std::shared_ptr<spdlog::logger> ClientLogger;
	};

	static LoggerState s_State;

	bool InitializaLogging()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Anomaly.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_State.CoreLogger = std::make_shared<spdlog::logger>("Anomaly", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_State.CoreLogger);
		s_State.CoreLogger->set_level(spdlog::level::trace);
		s_State.CoreLogger->flush_on(spdlog::level::trace);

		s_State.ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_State.ClientLogger);
		s_State.ClientLogger->set_level(spdlog::level::trace);
		s_State.ClientLogger->flush_on(spdlog::level::trace);

		return true;
	}

	void ShutdownLogging()
	{
	}

	void LogOutput(log_level level, const char* message, ...)
	{
		const char* level_strings[6] = {"[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };

		const i32 msg_length = 32000;
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
			s_State.ClientLogger->error(out_message2);
			break;
		case log_level::LOG_LEVEL_WARN:
			s_State.ClientLogger->warn(out_message2);
			break;
		case log_level::LOG_LEVEL_INFO:
			s_State.ClientLogger->info(out_message2);
			break;
		case log_level::LOG_LEVEL_DEBUG:
			s_State.ClientLogger->debug(out_message2);
			break;
		case log_level::LOG_LEVEL_TRACE:
			s_State.ClientLogger->trace(out_message2);
			break;
		}
	}

	void LogOutputCore(log_level level, const char* message, ...)
	{
		const char* level_strings[6] = {"[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };

		const i32 msg_length = 32000;
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
			s_State.CoreLogger->error(out_message2);
			break;
		case log_level::LOG_LEVEL_WARN:
			s_State.CoreLogger->warn(out_message2);
			break;
		case log_level::LOG_LEVEL_INFO:
			s_State.CoreLogger->info(out_message2);
			break;
		case log_level::LOG_LEVEL_DEBUG:
			s_State.CoreLogger->debug(out_message2);
			break;
		case log_level::LOG_LEVEL_TRACE:
			s_State.CoreLogger->trace(out_message2);
			break;
		}
	}
}
