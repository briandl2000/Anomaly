#include "D3D12_Texture.h"

#include "D3D12_Core.h"

namespace Anomaly::graphics::d3d12
{
    D3D12_Texture::D3D12_Texture(std::string path)
    {
        m_SRV = core::GetSrvHeap().Allocate();

        m_Width = 100;
        m_Height = 100;

        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.MipLevels = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        textureDesc.DepthOrArraySize = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

        D3D12_HEAP_PROPERTIES heapProps{};
        heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;

        AWINDOWS_ASSERT(core::GetDevice()->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&m_Resource)));

        // texture data
        std::vector<u8> texture= std::vector<u8>(m_Width * m_Height * 4);
        for(u64 y = 0; y < m_Height; y++)
        {
            for(u64 x = 0; x < m_Width; x++)
            {
                u64 index = (x+y*m_Width)*4;
                texture[index + 0] = (u8)((float)(x)/(float)(m_Width) * 255);
                texture[index + 1] = (u8)((float)(y)/(float)(m_Height) * 255);
                texture[index + 2] = 0;
                texture[index + 3] = 255;
            }
        }

        D3D12_SUBRESOURCE_DATA textureData = {};
        textureData.pData = &texture[0];
        textureData.RowPitch = m_Width * 4;
        textureData.SlicePitch = textureData.RowPitch * m_Height;

        core::UploadTextureData(m_Resource.Get(), &textureData);

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        core::GetDevice()->CreateShaderResourceView(m_Resource.Get(), &srvDesc, m_SRV.CPU);
    }

    
    D3D12_Texture::~D3D12_Texture()
    {
        core::GetSrvHeap().Free(m_SRV);
    }
    
    u32 D3D12_Texture::GetWidth()
    {
        return m_Width;
    }
    u32 D3D12_Texture::GetHeight()
    {
        return m_Height;
    }

    eTextureFormat D3D12_Texture::GetFormat()
    {
        return eTextureFormat::R8G8B8A8_UINT;
    }

    void* D3D12_Texture::GetHandle()
    {
        return (void*)(m_SRV.GPU.ptr);
    }

}
