#pragma once

#include "EngineDefines.h"

#include "CommonHeaders.h"

namespace Anomaly
{
    
    struct WindowDesc
    {
        std::string name{""};
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
        virtual std::string GetName() = 0;

        static Ref<Window> CreateWin(const WindowDesc& windowDesc);
    };    
} 
