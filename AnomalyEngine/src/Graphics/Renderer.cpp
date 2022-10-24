#include "Renderer.h"

#include "GraphicsPlatformInterface.h"

#include "CommonHeaders.h"

#ifdef PLATFORM_WINDOWS
#include "Platform/D3D12/D3D12_Interface.h"
#endif

namespace Anomaly::graphics
{
    namespace 
    {
        AString GraphicsAPIToString(const GraphicsAPI& graphicsAPI)
        {
            switch (graphicsAPI)
            {
            case GraphicsAPI::None: return "None";
            case GraphicsAPI::D3D12: return "D3D12";
            case GraphicsAPI::OpenGL: return "OpenGL";
            }
        }

        struct RendererState
        {
            GraphicsAPI SelectedAPI{GraphicsAPI::None};

            GraphicsPlatformInterface gfx;
        };

        static Scope<RendererState> s_State;

        bool SetGraphicsPlatformInterface()
        {
            switch (s_State->SelectedAPI)
            {
            default:
            {
                AERROR("Graphics API: %s is not supported!", GraphicsAPIToString(s_State->SelectedAPI).c_str());
                return false;
            }
            case GraphicsAPI::D3D12:
            {
                #ifdef PLATFORM_WINDOWS
                d3d12::GetGraphicsPlatfromInterface(s_State->gfx);
                #endif
            } break;
            }

            return s_State->gfx.Initialize();
        }
    }

    bool Initialize(const GraphicsAPI& graphicsAPI) 
    {
        s_State = CreateScope<RendererState>();
        s_State->SelectedAPI = graphicsAPI;
        bool result = SetGraphicsPlatformInterface();
        if(!result)
        {
            return false;
        }
        return true;
    }

    void Shutdown() 
    {
        s_State->gfx.Shutdown();
        s_State.release();
    }

    void RenderSurface(Window* windowIndex)
    {
        s_State->gfx.RenderSurface(windowIndex);
    }

    Surface* CreateSurface(Window* windowIndex)
    {
        return s_State->gfx.CreateSurface(windowIndex);
    }

    void DestroySurface(Window* windowIndex)
    {
        if(s_State)
        {
            s_State->gfx.DestroySurface(windowIndex);
        }
    }

    GraphicsAPI GetAPI() 
    {
        return s_State->SelectedAPI;
    }

    

}
