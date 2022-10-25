#include "Input.h"

#include "CommonHeaders.h"

namespace Anomaly::input
{
    
    namespace
    {
        #define MAX_KEYS 256
        #define MAX_MOUSE_BUTTONS 5
        struct KeyboardState
        {
            bool keys[MAX_KEYS]{false};
        };

        struct MouseState
        {
            i16 x{0};
            i16 y{0};
            bool buttons[MAX_MOUSE_BUTTONS]{false};
        };

        struct InputState
        {
            KeyboardState currentKeyboard;
            KeyboardState previousKeyboard;
            MouseState currentMouse;
            MouseState previousMouse;
        };

        static Scope<InputState> s_State;
    }

    bool Initialize()
    {
        s_State = CreateScope<InputState>();
        return true;
    }

    void Shutdown()
    {
        s_State = nullptr;
    }

    void Update(f64 deltaTime)
    {
        if(!s_State)
            return;

        memcpy(&s_State->previousKeyboard, &s_State->currentKeyboard, sizeof(KeyboardState));
        memcpy(&s_State->previousMouse, &s_State->currentMouse, sizeof(MouseState));
    }

    void ProcessKey(eKeys key, bool pressed)
    {  
        if(s_State && s_State->currentKeyboard.keys[key] != pressed)
        {
            s_State->currentKeyboard.keys[key] = pressed;

            event::EventData data{};
            data.sender = nullptr;
            data.data.u16[0] = key;
            event::FireEvent(pressed ? event::EVENT_CODE_KEY_PRESSED : event::EVENT_CODE_KEY_RELEASED, data);
        }
    }

    void ProcessButton(eMouseButtons button, bool pressed)
    {
        if(s_State && s_State->currentMouse.buttons[button] != pressed)
        {
            s_State->currentMouse.buttons[button] = pressed;

            event::EventData data{};
            data.sender = nullptr;
            data.data.u16[0] = button;
            event::FireEvent(pressed ? event::EVENT_CODE_BUTTON_PRESSED : event::EVENT_CODE_BUTTON_PRESSED, data);
        }
    }

    void PrcessMouseMove(i16 x, i16 y)
    {
        if(s_State && s_State->currentMouse.x != x || s_State->currentMouse.y != y)
        {
            s_State->currentMouse.x = x;
            s_State->currentMouse.y = y;

            event::EventData data{};
            data.sender = nullptr;
            data.data.u16[0] = x;
            data.data.u16[1] = y;
            event::FireEvent(event::EVENT_CODE_MOUSE_MOVED, data);
        }
    }

    void ProcessMouseWheel(i8 zDelta)
    {
        event::EventData data{};
        data.sender = nullptr;
        data.data.u8[0] = zDelta;
        event::FireEvent(event::eSystemEventCode::EVENT_CODE_MOUSE_WHEEL, data);
    }

    bool IsKeyDown(eKeys key)
    {
        if(!s_State)
            return false;

        return s_State->currentKeyboard.keys[key] == true; 
    }

    bool IsKeyUp(eKeys key)
    {
        if(!s_State)
            return true;

        return s_State->currentKeyboard.keys[key] == false;
    }

    bool WasKeyDown(eKeys key)
    {
        if(!s_State)
            return false;

        return s_State->previousKeyboard.keys[key] == true;
    }

    bool WasKeyUp(eKeys key)
    {
        if(!s_State)
            return true;

        return s_State->previousKeyboard.keys[key] == false;
    }


    bool IsButtonDown(eMouseButtons button)
    {
        if(!s_State)
            return false;

        return s_State->currentMouse.buttons[button] == true;
    }

    bool IsButtonUp(eMouseButtons button)
    {
        if(!s_State)
            return true;

        return s_State->currentMouse.buttons[button] == false;
    }

    bool WasButtonDown(eMouseButtons button)
    {
        if(!s_State)
            return false;

        return s_State->previousMouse.buttons[button] == true;
    }

    bool WasButtonUp(eMouseButtons button)
    {
        if(!s_State)
            return true;

        return s_State->previousMouse.buttons[button] == false;
    }


    void GetMousePosition(i32* x, i32* y)
    {
        if(!s_State)
        {
            *x = 0;
            *y = 0;
            return;
        }
        *x = s_State->currentMouse.x;
        *y = s_State->currentMouse.y;
    }

    void GetPreviousMousePosition(i32* x, i32* y)
    {
        if(!s_State)
        {
            *x = 0;
            *y = 0;
            return;
        }
        *x = s_State->previousMouse.x;
        *y = s_State->previousMouse.y;
    }

}