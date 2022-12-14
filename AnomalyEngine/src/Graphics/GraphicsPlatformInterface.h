#pragma once

#include "EngineDefines.h"

namespace Anomaly
{
    class Window;
}

namespace Anomaly::graphics
{
    class Surface;
    struct GraphicsPlatformInterface
    {
        bool (*Initialize)();
        void (*Shutdown)();
        void (*RenderSurface)(Window* windowIndex);
        Surface* (*CreateSurface)(Window* windowIndex);
        void (*DestroySurface)(Window* windowIndex);
    };
}