#include "Event.h"

#include "Logger.h"

#include <string>
#include <vector>

namespace Anomaly::event
{

    namespace
    {
        // TODO: create a system that will block events on systems on a lower level.
        class Event
        {
        public:
            Event() {};
            ~Event() {};

            void AddListener(IEventCallback* eventCallback)
            {
                if(m_EventCallbacks.size() == 0)
                {
                    m_EventCallbacks.push_back(eventCallback);
                    return;
                }
                
                CallbackArray::iterator position = std::find(m_EventCallbacks.begin(), m_EventCallbacks.end(), eventCallback);

                if(position != m_EventCallbacks.end())
                {
                    AWARN("Event callback already exist in this event");
                    return;
                }

                m_EventCallbacks.push_back(eventCallback);
            }

            void RemoveListener(IEventCallback* eventCallback)
            {
                if(m_EventCallbacks.size() == 0)
                {
                    return;
                }

                CallbackArray::iterator position = std::find(m_EventCallbacks.begin(), m_EventCallbacks.end(), eventCallback);

                if(position == m_EventCallbacks.end())
                {
                    return;
                }

                m_EventCallbacks.erase(position);
            }

            void Fire(const EventData& data)
            {
                for(IEventCallback* eventCallback : m_EventCallbacks)
                {
                    (*eventCallback)(data);
                }
            }
        private:
            typedef std::vector<IEventCallback*> CallbackArray;
            CallbackArray m_EventCallbacks;
        };

        #define MAX_MESSAGE_CODES 16384

        struct EventSystemState
        {
            Event registeredEvents[MAX_MESSAGE_CODES];
        };

        static Scope<EventSystemState> s_State;
    }

    bool Initialize()
    {
        s_State = CreateScope<EventSystemState>();

        return true;
    }

    void Shutdown()
    {
        s_State.release();
    }

    void EventRegister(u32 code, IEventCallback* eventCallback)
    {
        if(!s_State)
        {
            AWARN("Event system not initialized!");
            return;
        }
        s_State->registeredEvents[code].AddListener(eventCallback);
    }
    
    void EventUnregister(u32 code, IEventCallback* eventCallback)
    {
        if(!s_State)
            return;
        s_State->registeredEvents[code].RemoveListener(eventCallback);
    }

    void FireEvent(u32 code, const EventData& data)
    {
        EventData SendData = data;
        SendData.code = code;
        s_State->registeredEvents[code].Fire(SendData);
    }

}
