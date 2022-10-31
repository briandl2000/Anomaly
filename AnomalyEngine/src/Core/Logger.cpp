#include "Logger.h"
#include "Asserts.h"

#include "Platform/Platform.h"

#include <stdarg.h>
#include <stdio.h>

#include "imgui.h"

namespace Anomaly:: logger
{
    namespace
    {
        struct LogDataHeader
        {
            u64 length;
            eLogLevel level;
        };

        struct LoggerState
        {
            char* LoggerData{nullptr};
            u64 LoggerDataPosition{0};
        };

        static Scope<LoggerState> s_State{nullptr};
    }

    bool Initialize()
    {
        s_State = CreateScope<LoggerState>();
        s_State->LoggerData = new char[100000000]{0};

        return true;
    }
    
    void Shutdown()
    {
        delete[] s_State->LoggerData;
        s_State = nullptr;
    }

    void LogOutput(eLogLevel level, std::string message, ...)
    {
        const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: "};
        bool is_error = level < eLogLevel::LOG_LEVEL_WARN;
        message = level_strings[level] + message + "\n";

        char buffer[32000];
        
        va_list arg_ptr;
        va_start(arg_ptr, message);
        vsnprintf(buffer, 32000, message.c_str(), arg_ptr);
        va_end(arg_ptr);

        u64 length = (u64)(strlen(buffer))+1;
        LogDataHeader dataHeader;
        dataHeader.length = length;
        dataHeader.level = level;
        memcpy(s_State->LoggerData + s_State->LoggerDataPosition, &dataHeader, sizeof(LogDataHeader));
        s_State->LoggerDataPosition += sizeof(LogDataHeader);
        memcpy(s_State->LoggerData + s_State->LoggerDataPosition, buffer, length);
        s_State->LoggerDataPosition += length;

        platform::ConsoleWrite(buffer, level);
    }

    void ReportAssertionFailure(const char* expression, const char* message, const char* file, i32 line) 
    {
        LogOutput(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
    }

    // TODO: Figure out a better way to output this info in imgui

    bool GetNextLogData(u64& logPtrLocation, LogDataHeader& logDataHeader)
    {
        logPtrLocation += sizeof(LogDataHeader) + logDataHeader.length;
        memcpy(&logDataHeader, s_State->LoggerData + logPtrLocation, sizeof(LogDataHeader));
        if(logDataHeader.length == 0) return false;
        return true;
    }

    ImVec4 GetColor(eLogLevel level)
    {
        switch (level)
        {
        case LOG_LEVEL_FATAL: return ImVec4(1.f, 0.f, 0.f, 1.f);
        case LOG_LEVEL_ERROR: return ImVec4(1.f, 0.f, 0.f, 1.f);
        case LOG_LEVEL_WARN: return ImVec4(1.f, 1.f, 0.f, 1.f);
        case LOG_LEVEL_INFO: return ImVec4(0.f, 1.f, 0.f, 1.f);
        case LOG_LEVEL_DEBUG: return ImVec4(0.4f,0.4f, 1.f, 1.f);
        case LOG_LEVEL_TRACE: return ImVec4(0.f,0.f, 1.f, 1.f);
        }
        return ImVec4(0.f, 0.f, 0.f, 0.f);
    }

    void ImGuiDraw()
    {
        u64 logPtrLocation{0};
        LogDataHeader logDataHeader;
        memcpy(&logDataHeader, s_State->LoggerData + logPtrLocation, sizeof(LogDataHeader));
        while(GetNextLogData(logPtrLocation, logDataHeader))
        {
            auto col = GetColor(logDataHeader.level);
            ImGui::PushStyleColor(ImGuiCol_Text, col);
            ImGui::Text(s_State->LoggerData + logPtrLocation + sizeof(LogDataHeader));
            ImGui::PopStyleColor();
        }
    }

}