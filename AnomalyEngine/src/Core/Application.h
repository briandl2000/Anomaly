#pragma once

#include "EngineDefines.h"

#include "CommonHeaders.h"

namespace Anomaly
{
    // Forward decleration
    class Window;
    namespace event
    {
        struct EventData;
        template<typename T>
        class EventCallback;
    }

    // Application class
    class Application
    {
        ADD_EVENT_LISTENERS(Application)
    public:
        Application();
        virtual ~Application();

        virtual void OnInit() {};
        virtual void OnShutdown() {};
        virtual void OnUpdate() {};
        void Run();

        void InitializeSystems();
        void ShutdownSystems();

        bool OnWindowClose(const event::EventData& data);
        bool OnWindowResize(const event::EventData& data);
    private:

        bool m_ShouldClose{false};
        
        Ref<Window> m_Window{nullptr};


    };
}