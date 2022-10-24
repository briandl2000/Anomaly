#include "Logger.h"
#include "Asserts.h"

#include "Platform/Platform.h"

#include <stdarg.h>
#include <stdio.h>

namespace Anomaly:: logger
{
    bool Initialize()
    {
        return true;
    }
    
    void Shutdown()
    {
    }

    void LogOutput(eLogLevel level, AString message, ...)
    {
        const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: "};
        bool is_error = level < eLogLevel::LOG_LEVEL_WARN;
        message = level_strings[level] + message + "\n";

        char buffer[32000];
        
        va_list arg_ptr;
        va_start(arg_ptr, message);
        vsnprintf(buffer, 32000, message.c_str(), arg_ptr);
        va_end(arg_ptr);

        platform::ConsoleWrite(buffer, level);
    }

    void ReportAssertionFailure(const char* expression, const char* message, const char* file, i32 line) {
        LogOutput(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
    }

}