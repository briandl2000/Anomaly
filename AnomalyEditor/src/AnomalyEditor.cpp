#include "Core/EntryPoint.h"

#include "CommonHeaders.h"
#include "Platform/Window.h"
#include "Graphics/Renderer.h"

namespace Anomaly
{
    class Test
    {
        ADD_EVENT_LISTENERS(Test)
    public:
        Test(int pos)
        {
            AddEventListener(event::eSystemEventCode::EVENT_CODE_RESIZED, &Test::ResizeCallback);
            AddEventListener(event::eSystemEventCode::EVENT_CODE_WINDOW_CLOSED, &Test::WindowCloseCallback);
            m_Window = Window::CreateWin({"Test Window", 200, 200, pos, pos});
        }
        ~Test() 
        {
            m_Window = nullptr;
        }

        bool ResizeCallback(const event::EventData& data)
        {
            return false;
        }
        bool WindowCloseCallback(const event::EventData& data)
        {
            if(data.sender == m_Window.get())
            {
                m_Window = nullptr;
            }
            return false;
        }

        void Render()
        {
            graphics::RenderSurface(m_Window.get());
        }
    private:

        Ref<Window> m_Window{nullptr};

    };

    class EditorApp : public Application
    {
        ADD_EVENT_LISTENERS(EditorApp)
    public:
        EditorApp()
        {
            AddEventListener(event::eSystemEventCode::EVENT_CODE_KEY_PRESSED, &EditorApp::KeyPressedCallback);
        }

        virtual ~EditorApp() override
        {
        }
        
        virtual void OnUpdate() override
        {
            for(auto& test : tests)
            {
                test.Render();
            }
        }

        bool KeyPressedCallback(const event::EventData& data)
        {
            if(data.data.u16[0] == input::eKeys::KEY_A)
            {
                for(int i = 0; i < 10; i++)
                {
                    tests.push_back({pos});
                    pos += 10;
                }
            }
            else if(data.data.u16[0] == input::eKeys::KEY_D)
            {
                if(tests.size() > 0)
                {
                    for(int i = 0; i < 10; i++)
                    {
                        tests.pop_back();
                        pos -= 10;
                    }
                }
            }
            return false;
        }

    private:
        int pos = 100;
        AVector<Test> tests{};
    };

}
Anomaly::Application* CreateApplication()
{
    return new Anomaly::EditorApp();
}