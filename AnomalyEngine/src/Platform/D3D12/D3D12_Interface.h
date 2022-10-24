#pragma once 

#include "EngineDefines.h"

namespace Anomaly::graphics
{
    struct GraphicsPlatformInterface;

    namespace d3d12
    {
        void GetGraphicsPlatfromInterface(GraphicsPlatformInterface& gfx);
    }
}