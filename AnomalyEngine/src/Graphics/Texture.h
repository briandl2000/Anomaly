#pragma once

#include "CommonHeaders.h"

#include "RenderUtils.h"

namespace Anomaly::graphics
{
    class Texture
    {
    public:

        virtual ~Texture() {};
        
        virtual u32 GetWidth() = 0;
        virtual u32 GetHeight() = 0;

        virtual eTextureFormat GetFormat() = 0;

        virtual void* GetHandle() = 0;

        static Ref<Texture> CreateTexture(std::string path);
    private:    

    };
}