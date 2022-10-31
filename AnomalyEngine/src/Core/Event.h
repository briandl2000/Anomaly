#pragma once

#include "EngineDefines.h"


namespace Anomaly::event
{
    enum eSystemEventCode
    {
        EVENT_CODE_NONE = 0x00,
        EVENT_CODE_WINDOW_CLOSED = 0x01,
        EVENT_CODE_KEY_PRESSED = 0x02,
        EVENT_CODE_KEY_RELEASED = 0x03,
        EVENT_CODE_BUTTON_PRESSED = 0x04,
        EVENT_CODE_BUTTON_RELEASED = 0x05,
        EVENT_CODE_MOUSE_MOVED = 0x06,
        EVENT_CODE_MOUSE_WHEEL = 0x07,
        EVENT_CODE_RESIZED = 0x08,

        EVENT_CODE_IMGUI_EVENT = 0x09,

        MAX_EVENT_CODE = 0xFF
    };

    struct EventData
    {
        u32 code{EVENT_CODE_NONE};
        void* sender;
        union
        {
            i64 i64[2];
            u64 u64[2];
            f64 f64[2];

            i32 i32[4];
            u32 u32[4];
            f32 f32[4];

            i16 i16[8];
            u16 u16[8];

            i8 i8[16];
            u8 u8[16];

            char c[16];
        } data;
    };

    bool Initialize();

    void Shutdown();

    class IEventCallback
    {
    public:
        virtual bool operator() (const EventData& data) = 0;
        virtual bool operator == (IEventCallback* other) = 0;
    };

    void EventRegister(u32 code, IEventCallback* eventCallback);
    void EventUnregister(u32 code, IEventCallback* eventCallback);
    void FireEvent(u32 code, const EventData& data);

    template<typename T>
    class EventCallback : public IEventCallback
    {
    public:
        EventCallback(u32 code, T* listener, bool (T::*callback)(const EventData& data))
            : m_Listener(listener)
            , m_Callback(callback)
            , m_Code(code)
        {
            EventRegister(m_Code, this);
        }

        ~EventCallback()
        {
            EventUnregister(m_Code, this);
        }

        virtual bool operator () (const EventData& data) override 
        { 
            return (m_Listener->*m_Callback)(data);
        }

        virtual bool operator == (IEventCallback* other) override
        {
            EventCallback* otherEventCallback = dynamic_cast<EventCallback*>(other);

            if(otherEventCallback == nullptr)
                return false;
            
            return (this->m_Callback == otherEventCallback->m_Callback) &&
                   (this->m_Listener == otherEventCallback->m_Listener);
        }

    private:
        bool (T::*m_Callback)(const EventData& data);
        T* m_Listener;
        u32 m_Code;
    };

}

#define ADD_EVENT_LISTENERS(className)                                                                  \
private:                                                                                                \
typedef Anomaly::event::EventCallback<className> EventListener;                                         \
std::vector<Anomaly::Ref<EventListener>> m_EventListeners;                                                  \
void AddEventListener(u32 code, bool(className::*callback)(const Anomaly::event::EventData& data))      \
{                                                                                                       \
    m_EventListeners.push_back(                                                                         \
        Anomaly::CreateRef<EventListener>(code, this, callback)                                         \
    );                                                                                                  \
}