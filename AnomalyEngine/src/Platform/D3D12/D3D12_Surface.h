#pragma once

#include "D3D12_CommonHeaders.h"

#include "Graphics/RenderSurface.h"
#include "D3D12_Resources.h" 


namespace Anomaly::graphics::d3d12
{
    class D3D12_Surface : public Surface
    {
    public:
        D3D12_Surface(Window* window);
        virtual ~D3D12_Surface();

        virtual void Resize()  override;
        virtual void SwapBuffers() override;
        virtual u32 GetWidth() override;
        virtual u32 GetHeight() override;

        void CreateSwapchain(DXGI_FORMAT format);

        ID3D12Resource* GetResource() { return m_RenderTargetData[m_CurrentBackBufferIndex].Resource.Get(); }
        const D3D12_CPU_DESCRIPTOR_HANDLE& GetRtv() const { return m_RenderTargetData[m_CurrentBackBufferIndex].rtv.CPU; }
        const D3D12_VIEWPORT& GetViewPort() const {return m_ViewPort;};
        const D3D12_RECT& GetRect() const {return m_ScissorRect;}

    private:
        void Finalize();

        struct RenderTargetData
        {
            ComPtr<ID3D12Resource> Resource{nullptr};
            DescriptorHandle rtv{};
        };

        Window* m_Window;
        RenderTargetData m_RenderTargetData[c_NumBackBuffers]{};

        u32 m_CurrentBackBufferIndex{0};

        D3D12_VIEWPORT m_ViewPort{};
        D3D12_RECT m_ScissorRect{};

        ComPtr<IDXGISwapChain4> m_Swapchain{nullptr};
        DXGI_FORMAT m_Format;
    };
}
    