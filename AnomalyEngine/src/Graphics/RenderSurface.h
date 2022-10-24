#pragma once

#include "CommonHeaders.h"

namespace Anomaly
{
    class Window;
}
namespace Anomaly::graphics
{
    
    class Surface
    {
    public:
        static constexpr u32 c_NumBackBuffers{3};

        virtual ~Surface() {};

        virtual void Resize(u32 width, u32 height) = 0;
        virtual void SwapBuffers() = 0;
        virtual u32 GetWidth() = 0;
        virtual u32 GetHeight() = 0;
        void SetIndex(u32 index) {m_Index = index;};
        u32 GetIndex() const { return m_Index; };
    private:
        u32 m_Index{0};
    };

}
