#include "Core/EntryPoint.h"

#include "CommonHeaders.h"
#include "Platform/Window.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"

#include "imgui.h"

namespace Anomaly
{
    class EditorApp : public Application
    {
    public:
        EditorApp(int argc, char** argv)
        {
            for(int i = 0; i < argc; i++)
            {
                ADEBUG("Arg %i: %s", i, argv[i]);
            }

            texture = graphics::Texture::CreateTexture("");
        }

        virtual ~EditorApp() override
        {
        }

        virtual void OnImGui()
        {
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;
            
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", nullptr, window_flags);
            ImGui::PopStyleVar(3);

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem("Debug Info", "", debugInfoOpen))
                    { 
                        debugInfoOpen = !debugInfoOpen;
                    }


                    ImGui::EndMenu();
                }
                
                ImGui::EndMenuBar();
            }

            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
            ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoDecoration);
            
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            ImGui::Image((ImTextureID)texture->GetHandle(), viewportPanelSize, ImVec2(0., 1.), ImVec2(1., 0.));
            
            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::Begin("Scene hierarchy");
            ImGui::End();

            ImGui::Begin("Asset browser");
            ImGui::End();

            ImGui::Begin("Output logger");
            static char inputText[128]{0};
            ImGui::InputText("Filter", inputText, 128);
            ImGui::Separator();
            ImGui::BeginChild("logger", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            logger::ImGuiDraw();
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
            ImGui::End();

            ImGui::Begin("Properties");
            ImGui::End();
            
            if(debugInfoOpen)
            {
                ImGui::Begin("Info", &debugInfoOpen);
                ImGui::End();
            }
        }

    private:
        bool debugInfoOpen{false};

        Ref<graphics::Texture> texture;
    };

}
Anomaly::Application* CreateApplication(int argc, char** argv)
{
    return new Anomaly::EditorApp(argc, argv);
}