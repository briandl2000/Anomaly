#include "Application.h"

#include "Platform/Platform.h"
#include "Platform/Window.h"

#include "Graphics/Renderer.h"

#include "Input.h"

#include "imgui.h"

namespace Anomaly
{
    Application::Application()
    {
        if(!logger::Initialize())
        {
            AERROR("Logger System Failed to initialize!");
        }
        AINFO("Logger initialized.");

        AINFO("Initializing the event system...");
        if(!event::Initialize())
        {
            AERROR("Event System Failed to initialize!");
        }

        AINFO("Initializing the platform system...");
        if(!platform::Initialize())
        {
            AERROR("Platform System Failed to initialize!");
        }

        AINFO("Initializing the platform system...");
        if(!input::Initialize())
        {
            AERROR("Platform System Failed to initialize!");
        }

        AINFO("Initializing the event graphics system...");
        // TODO: initialize graphics based on platform
        if(!graphics::Initialize(graphics::eGraphicsAPI::D3D12))
        {
            AERROR("Renderer System Failed to initialize!");
        }
        
        AddEventListener(event::eSystemEventCode::EVENT_CODE_WINDOW_CLOSED, &Application::OnWindowClose);
        AddEventListener(event::eSystemEventCode::EVENT_CODE_RESIZED, &Application::OnWindowResize);
        AddEventListener(event::eSystemEventCode::EVENT_CODE_IMGUI_EVENT, &Application::OnImGui);

        m_Window = Window::CreateWin({"My Window", 900, 600, 100, 100});
    }
    
    Application::~Application()
    {
        AINFO("Shutting down the graphics system...");
        graphics::Shutdown();

        AINFO("Shutting down the input system...");
        input::Shutdown();

        AINFO("Shutting down the platform system...");
        platform::Shutdown();

        AINFO("Shutting down the event system...");
        event::Shutdown();

        AINFO("Shutting down the logger system...");
        logger::Shutdown();
    }

    void Application::Run()
    {
        OnInit();
        while(!m_ShouldClose)
        {
            platform::PumpMessage();
            if(m_ShouldClose)
                break;

            OnUpdate();

            graphics::RenderSurface(m_Window.get());
        }
        OnShutdown();
    }

    bool Application::OnWindowClose(const event::EventData& data) 
    {
        if(data.sender == m_Window.get())
            m_ShouldClose = true;
        
        return true;
    }

    bool Application::OnWindowResize(const event::EventData& data) 
    {   
        Window* window = (Window*)data.sender;
        //AINFO("Window: %s reisized to %i width and %i height", window->GetName().c_str(), window->GetWidth(), window->GetHeight());
        return true;
    }

    bool Application::OnImGui(const event::EventData& data) 
    {
        OnImGui();

        return true;
    };
}