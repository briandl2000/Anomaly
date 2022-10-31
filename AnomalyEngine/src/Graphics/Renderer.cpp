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
        std::string GraphicsAPIToString(const eGraphicsAPI& graphicsAPI)
        {
            switch (graphicsAPI)
            {
            case eGraphicsAPI::None: return "None";
            case eGraphicsAPI::D3D12: return "D3D12";
            case eGraphicsAPI::OpenGL: return "OpenGL";
            }

            AASSERT("Unknown graphics API selected!");
            return "";
        }

        struct RendererState
        {
            eGraphicsAPI SelectedAPI{eGraphicsAPI::None};

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
            case eGraphicsAPI::D3D12:
            {
                #ifdef PLATFORM_WINDOWS
                d3d12::GetGraphicsPlatfromInterface(s_State->gfx);
                #endif
            } break;
            }

            return s_State->gfx.Initialize();
        }
    }

    bool Initialize(const eGraphicsAPI& graphicsAPI) 
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

    eGraphicsAPI GetAPI() 
    {
        return s_State->SelectedAPI;
    }

    

}
