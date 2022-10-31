#include "Texture.h"

#include "Renderer.h"

#ifdef PLATFORM_WINDOWS
#include "Platform/D3D12/D3D12_Texture.h"
#endif

namespace Anomaly::graphics
{
    
    Ref<Texture> Texture::CreateTexture(std::string path)
    {
        switch (GetAPI())
        {
            #ifdef PLATFORM_WINDOWS
            case eGraphicsAPI::D3D12: return CreateRef<d3d12::D3D12_Texture>(path);
            #endif
        };
        
        AASSERT_MSG(false, "Could not create Texture!");
        return nullptr;
    }

} 
