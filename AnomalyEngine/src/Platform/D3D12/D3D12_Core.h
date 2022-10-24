#pragma once

#include "EngineDefines.h"

#include "D3D12_CommonHeaders.h"

namespace Anomaly
{
    namespace graphics
    {
        class Surface;
        namespace d3d12
        {
            class DescriptorHeap;
        }
    }
}
namespace Anomaly::graphics::d3d12::core
{

    DXGI_FORMAT GetDefaultRenderTargetFormat();

    bool Initialize();

    void Shutdown();

    void RenderSurface(Window* index);
    Surface* CreateSurface(Window* index);
    void DestroySurface(Window* index);

    void CreateSurface();

    ComPtr<ID3D12Device8> const GetDevice();

    void Flush();

    u32 CurrentFrameIndex();
    void SetDeferredReleasesFlag();

    ComPtr<IDXGISwapChain4> CreateSwapchain(Window* window, const DXGI_SWAP_CHAIN_DESC1& desc);

    DescriptorHeap& GetRtvHeap();
    DescriptorHeap& GetDsvHeap();
    DescriptorHeap& GetSrvHeap();
    DescriptorHeap& GetUavHeap();
}
