#include "Platform/Window.h"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#undef WIN32_LEAN_AND_MEAN

namespace Anomaly
{
    namespace graphics
    {
        class Surface;
    } 
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowDesc& windowDesc);
        virtual ~WindowsWindow() override;

        virtual i32 GetWidth() override;
        virtual i32 GetHeight() override;
        virtual std::string GetName() override;

        void Resize(u32 width, u32 height);

        HWND GetHandle() { return m_WindowHandle; }

    private:
        HWND m_WindowHandle{nullptr};

        i32 m_Width;
        i32 m_Height;

        i32 m_X;
        i32 m_Y;

        std::string m_Name;

        graphics::Surface* m_Surface{nullptr};
    };
}