#pragma once

#include "EngineDefines.h"

#include "Core/DataStructures.h"
#include "CommonHeaders.h"

namespace Anomaly
{
    
    struct WindowDesc
    {
        AString name{""};
        i32 width{0};
        i32 height{0};
        i32 startX{0};
        i32 startY{0};
    };

    class Window
    {
    public:
        virtual ~Window() {};

        ANOM_API virtual i32 GetWidth() = 0;
        ANOM_API virtual i32 GetHeight() = 0;
        ANOM_API virtual AString GetName() = 0;

        ANOM_API static Ref<Window> CreateWin(const WindowDesc& windowDesc);
    };    
} 
