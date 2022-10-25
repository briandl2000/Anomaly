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

        virtual i32 GetWidth() = 0;
        virtual i32 GetHeight() = 0;
        virtual AString GetName() = 0;

        static Ref<Window> CreateWin(const WindowDesc& windowDesc);
    };    
} 
