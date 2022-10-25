#include "D3D12_Surface.h"

#include "Platform/Window.h"

#include "D3D12_Core.h"

namespace Anomaly::graphics::d3d12
{

    namespace
    {
        DXGI_FORMAT ToNonSrgb(DXGI_FORMAT format)
        {
            if(format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) return DXGI_FORMAT_R8G8B8A8_UNORM;

            return format;
        }
    }

    D3D12_Surface::D3D12_Surface(Window* window)
    {
        m_Window = window;

        CreateSwapchain(DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    D3D12_Surface::~D3D12_Surface()
    {
        core::Flush();
        for(u32 i{0}; i < c_NumBackBuffers; i++)
        {
            RenderTargetData& data{m_RenderTargetData[i]};
            data.Resource = nullptr;
            core::GetRtvHeap().Free(m_RenderTargetData[i].rtv);
        }
    }

    void D3D12_Surface::Resize(u32 width, u32 height) 
    {
        core::Flush();
        for(u32 i{0}; i < c_NumBackBuffers; i++)
        {
            RenderTargetData& data{m_RenderTargetData[i]};
            data.Resource = nullptr;
        }

        DXGI_SWAP_CHAIN_DESC1 desc{};
        AWINDOWS_ASSERT(m_Swapchain->GetDesc1(&desc));
        AWINDOWS_ASSERT(m_Swapchain->ResizeBuffers(c_NumBackBuffers, GetWidth(), GetHeight(), desc.Format, desc.Flags));
        m_CurrentBackBufferIndex = m_Swapchain->GetCurrentBackBufferIndex();
        Finalize();
    }

    // TODO: make this function const
    void D3D12_Surface::SwapBuffers()
    {
        AASSERT(m_Swapchain);
        AWINDOWS_ASSERT(m_Swapchain->Present(0, 0));
        m_CurrentBackBufferIndex = m_Swapchain->GetCurrentBackBufferIndex();
    }

    u32 D3D12_Surface::GetWidth() 
    {
        return m_Window->GetWidth();
    }

    u32 D3D12_Surface::GetHeight() 
    {
        return m_Window->GetHeight();
    }

    void D3D12_Surface::CreateSwapchain(DXGI_FORMAT format) 
    {
        m_Format = format;

        DXGI_SWAP_CHAIN_DESC1 desc{};
        desc.Width = GetWidth();
        desc.Height = GetHeight();
        desc.Format = ToNonSrgb(format);
        desc.Stereo = false;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = c_NumBackBuffers;
        desc.Scaling = DXGI_SCALING_STRETCH;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags = 0;

        m_Swapchain = core::CreateSwapchain(m_Window, desc);

        m_CurrentBackBufferIndex = m_Swapchain->GetCurrentBackBufferIndex();

        for(u32 i{0}; i < c_NumBackBuffers; i++)
        {
            m_RenderTargetData[i].rtv = core::GetRtvHeap().Allocate();
        }

        Finalize();
    }

    void D3D12_Surface::Finalize()
    {
        for(u32 i{0}; i < c_NumBackBuffers; i++)
        {
            RenderTargetData& data{m_RenderTargetData[i]};
            AASSERT(!data.Resource);
            AWINDOWS_ASSERT(m_Swapchain->GetBuffer(i, IID_PPV_ARGS(&data.Resource)));
            D3D12_RENDER_TARGET_VIEW_DESC desc{};
            desc.Format = core::GetDefaultRenderTargetFormat();
            desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            core::GetDevice()->CreateRenderTargetView(data.Resource.Get(), &desc, data.rtv.CPU);
        }

        DXGI_SWAP_CHAIN_DESC desc{};
        AWINDOWS_ASSERT(m_Swapchain->GetDesc(&desc));
        const u32 width{desc.BufferDesc.Width};
        const u32 height{desc.BufferDesc.Height};
        AASSERT(m_Window->GetWidth() == width && m_Window->GetHeight() == height);

        m_ViewPort.TopLeftX = 0.f;
        m_ViewPort.TopLeftY = 0.f;
        m_ViewPort.Width = (float)width;
        m_ViewPort.Height = (float)height;
        m_ViewPort.MinDepth = 0.f;
        m_ViewPort.MaxDepth = 1.f;

        m_ScissorRect = {0, 0, (i32)width, (i32)height};
    }
}