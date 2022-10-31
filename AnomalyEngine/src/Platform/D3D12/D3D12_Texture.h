#pragma once

#include "Graphics/Texture.h"

#include "D3D12_CommonHeaders.h"

#include "D3D12_Resources.h" 

namespace Anomaly::graphics::d3d12
{
    class D3D12_Texture : public Texture
    {
    public:
        D3D12_Texture(std::string path);

        virtual ~D3D12_Texture() override;
        
        virtual u32 GetWidth() override;
        virtual u32 GetHeight() override;

        virtual eTextureFormat GetFormat() override;

        virtual void* GetHandle() override;

    private:
        u32 m_Width{0};
        u32 m_Height{0};

        ComPtr<ID3D12Resource> m_Resource{nullptr};
        DescriptorHandle m_SRV{};
    };
}