#include "PlatformWindows.h"

#include "WindowsWindow.h"

#include "CommonHeaders.h"

namespace Anomaly::platform
{
    namespace
    {
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        constexpr char CLASS_NAME[]  = "Anom_Window_Class";

        struct platformState
        {
            HINSTANCE hInstance{nullptr};
        };

        static Scope<platformState> s_State;
    }

    bool Initialize()
    {

        s_State = CreateScope<platformState>();

        s_State->hInstance = GetModuleHandleA(0);

        HICON icon = LoadIcon(s_State->hInstance, IDI_APPLICATION);
        WNDCLASSA wc = { };
        wc.style = CS_DBLCLKS;  // Get double-clicks
        wc.lpfnWndProc   = WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance  = s_State->hInstance;
        wc.hIcon = icon;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // NULL; // Manage the cursor manually
        wc.hbrBackground = NULL;                   // Transparent
        wc.lpszClassName = CLASS_NAME;

        if(!RegisterClassA(&wc))
        {
            AERROR("Could not register window class!");
            return false;
        }

        return true;

    }
    
    void Shutdown()
    {
        s_State.release();
    }

    bool PumpMessage()
    {
        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        return true;
    }

    void ConsoleWrite(const char* message, u8 color)
    {
        HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

        static u8 levels[6] = {64, 4, 6, 2, 11, 8};
        SetConsoleTextAttribute(console_handle, levels[color]);
        OutputDebugStringA(message);
        u64 length = strlen(message);
        LPDWORD number_written = 0;
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
    }

    HWND windows::CreateHWND(const AString& name, i32& x, i32& y, i32& width, i32& height, WindowsWindow* window) 
    {
        u32 windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        windowStyle |= WS_MAXIMIZEBOX;
        windowStyle |= WS_MINIMIZEBOX;
        windowStyle |= WS_THICKFRAME;
        u32 windowExStyle = WS_EX_APPWINDOW;

        // Obtain the size of the border.
        RECT borderRect = {0, 0, 0, 0};
        AdjustWindowRectEx(&borderRect, windowStyle, 0, windowExStyle);
        x += borderRect.left;
        y += borderRect.top;
        width += borderRect.right - borderRect.left;
        height += borderRect.bottom - borderRect.top;

        HWND hwnd = CreateWindowExA(
            windowExStyle, CLASS_NAME, name.c_str(),
            windowStyle, x, y, width, height,
            0, 0, s_State->hInstance, window);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);


        bool shouldActivate = 1;
        i32 showWindowCommandFlags = shouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;

        ShowWindow(hwnd, showWindowCommandFlags);

        return hwnd;
    }
    
    namespace
    {
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            WindowsWindow* parentWindow = (WindowsWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            event::EventData data{};
            data.sender = parentWindow;
        
            switch (uMsg)
            {
            case WM_DESTROY:
            {
                AASSERT(parentWindow != nullptr);
                event::FireEvent(event::eSystemEventCode::EVENT_CODE_WINDOW_CLOSED, data);
                return 0;   
            } break;
            case WM_SIZE:
            {
                AASSERT(parentWindow != nullptr);
                RECT r;
                GetClientRect(hwnd, &r);
                u32 width = r.right - r.left;
                u32 height = r.bottom - r.top;

                parentWindow->Resize(width, height);

                data.data.u16[0] = (u16)width;
                data.data.u16[1] = (u16)height;
                event::FireEvent(event::eSystemEventCode::EVENT_CODE_RESIZED, data);
                
            } break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: 
            {
                bool pressed = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
                input::eKeys key = (input::eKeys)wParam;

                bool is_extended = (HIWORD(lParam) & KF_EXTENDED) == KF_EXTENDED;

                if (wParam == VK_MENU) {
                    key = is_extended ? input::eKeys::KEY_RALT : input::eKeys::KEY_LALT;
                } else if (wParam == VK_SHIFT) {
                    u32 left_shift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
                    u32 scancode = ((lParam & (0xFF << 16)) >> 16);
                    key = scancode == left_shift ? input::eKeys::KEY_LSHIFT : input::eKeys::KEY_RSHIFT;
                } else if (wParam == VK_CONTROL) {
                    key = is_extended ? input::eKeys::KEY_RCONTROL : input::eKeys::KEY_LCONTROL;
                }

                input::ProcessKey(key, pressed);

                return 0;
            } break;
            case WM_MOUSEMOVE: 
            {
                // Mouse move
                i32 x = GET_X_LPARAM(lParam);
                i32 y = GET_Y_LPARAM(lParam);

                // Pass over to the input subsystem.
                input::PrcessMouseMove(x, y);
            } break;
            case WM_MOUSEWHEEL: 
            {
                i32 z_delta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (z_delta != 0) {
                    // Flatten the input to an OS-independent (-1, 1)
                    z_delta = (z_delta < 0) ? -1 : 1;
                    input::ProcessMouseWheel(z_delta);
                }
            } break;
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
            {
                bool pressed = uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN;
                input::eMouseButtons mouse_button = input::eMouseButtons::BUTTON_MAX_BUTTONS;
                switch (uMsg) {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                        mouse_button = input::eMouseButtons::BUTTON_LEFT;
                        break;
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP:
                        mouse_button = input::eMouseButtons::BUTTON_MIDDLE;
                        break;
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                        mouse_button = input::eMouseButtons::BUTTON_RIGHT;
                        break;
                }

                // Pass over to the input subsystem.
                if (mouse_button != input::eMouseButtons::BUTTON_MAX_BUTTONS) {
                    input::ProcessButton(mouse_button, pressed);
                }
            } break;
            }
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}