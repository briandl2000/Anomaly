#pragma once

#include "CommonHeaders.h"

#include "Platform/Platform.h"

// windows include
#include <windows.h>
#include <windowsx.h>

namespace Anomaly
{
    class WindowsWindow;
}

namespace Anomaly::platform::windows
{
    HWND CreateHWND(const AString& name, i32& x, i32& y, i32& width, i32& height, WindowsWindow* window);
}