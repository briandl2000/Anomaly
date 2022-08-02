#pragma once

#include "EngineDefines.h"
#include "Events/Event.h"

struct GLFWwindow;

namespace Anomaly
{

	struct WindowProps
	{
		char* Title;
		u32 Width;
		u32 Height;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window();
		~Window();

		u32 GetWidth();
		u32 GetHeight();
		void OnUpdate();

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; };

		void Init(const WindowProps& props);
		void Shutdown();
	private:
		GLFWwindow* m_Window;
		struct WindowData
		{
			char* Title;
			u32 Width, Height;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

	};
}
