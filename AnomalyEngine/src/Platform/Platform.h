#pragma once

#include "EngineDefines.h"

namespace Anomaly::platform
{
    bool Initialize();
    
    void Shutdown();

    bool PumpMessage();
 
    void ConsoleWrite(const char* message, u8 color);
}