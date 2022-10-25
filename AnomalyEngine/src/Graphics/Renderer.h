#pragma once

#include "EngineDefines.h"

namespace Anomaly
{
    class Window;
}

namespace Anomaly::graphics
{
    class Surface;
    enum class GraphicsAPI
    {
        None = 0,
        D3D12,
        OpenGL
    };

    bool Initialize(const GraphicsAPI& graphicsAPI = GraphicsAPI::None );

    void Shutdown();

    void RenderSurface(Window* windowIndex);
    
    Surface* CreateSurface(Window* windowIndex);
    void DestroySurface(Window* windowIndex);

    GraphicsAPI GetAPI();
} 