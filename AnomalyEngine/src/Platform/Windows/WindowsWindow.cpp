#include "WindowsWindow.h"

#include "CommonHeaders.h"

#include "PlatformWindows.h"

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>

#include "Graphics/Renderer.h"
#include "Graphics/RenderSurface.h"

namespace Anomaly
{

    WindowsWindow::WindowsWindow(const WindowDesc& windowDesc)
    {
        m_Name  = windowDesc.name;
        m_X = windowDesc.startX;
        m_Y = windowDesc.startY;
        m_Width = windowDesc.width;
        m_Height = windowDesc.height;

        m_WindowHandle = platform::windows::CreateHWND(m_Name, m_X, m_Y, m_Width, m_Height, this);

        m_Surface = graphics::CreateSurface(this);
    }

    WindowsWindow::~WindowsWindow()
    {
        graphics::DestroySurface(this);

        if (m_WindowHandle) {
            DestroyWindow(m_WindowHandle);
        }
    }

    i32 WindowsWindow::GetWidth()
    {
        return m_Width;
    }

    i32 WindowsWindow::GetHeight() 
    {
        return m_Height;
    }

    AString WindowsWindow::GetName() {
        return m_Name;
    }

    void WindowsWindow::Resize(u32 width, u32 height) {
        m_Width = width;
        m_Height = height;
        if(m_Surface)
        {
            m_Surface->Resize(width, height);
        }
    }
}
