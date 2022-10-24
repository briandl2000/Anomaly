#pragma once

#include "EngineDefines.h"

// Disable assertions by commenting out the below line.
#define AASSERTIONS_ENABLED

#ifdef AASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

namespace Anomaly::logger
{
    ANOM_API void ReportAssertionFailure(const char* expression, const char* message, const char* file, i32 line);
}

#define AASSERT(expr)                                                \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            Anomaly::logger::ReportAssertionFailure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }

#define AASSERT_MSG(expr, message)                                        \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            Anomaly::logger::ReportAssertionFailure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                 \
        }                                                                 \
    }

#ifdef _DEBUG
#define AASSERT_DEBUG(expr)                                          \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            Anomaly::logger::ReportAssertionFailure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }
#else
#define AASSERT_DEBUG(expr)  // Does nothing at all
#endif

#else
#define AASSERT(expr)               // Does nothing at all
#define AASSERT_MSG(expr, message)  // Does nothing at all
#define AASSERT_DEBUG(expr)         // Does nothing at all
#endif