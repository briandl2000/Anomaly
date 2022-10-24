#include "Window.h"

#include "CommonHeaders.h"

#ifdef PLATFORM_WINDOWS
#include "Windows/WindowsWindow.h"
#endif

namespace Anomaly
{

    Ref<Window> Window::CreateWin(const WindowDesc& windowDesc) 
    {
        #ifdef PLATFORM_WINDOWS
        return CreateRef<WindowsWindow>(windowDesc);
        #endif

        AASSERT_MSG(false, "Unkown platform selected");
        return nullptr;
    }

}
