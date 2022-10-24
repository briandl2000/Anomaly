#include "D3D12_Interface.h"

#include "Graphics/GraphicsPlatformInterface.h"

#include "D3D12_Core.h"

namespace Anomaly::graphics::d3d12
{

    void GetGraphicsPlatfromInterface(GraphicsPlatformInterface& gfx) 
    {    
        gfx.Initialize = core::Initialize;
        gfx.Shutdown = core::Shutdown;
        gfx.RenderSurface = core::RenderSurface;
        gfx.CreateSurface = core::CreateSurface;
        gfx.DestroySurface = core::DestroySurface;
    }
}