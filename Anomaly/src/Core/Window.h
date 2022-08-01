#pragma once

#include "EngineDefines.h"

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

		Window(const WindowProps& props);
		~Window();

		u32 GetWidth();
		u32 GetHeight();
		void OnUpdate();

	private:
		void Init(const WindowProps& props);
		void ShutDown();
	private:
		GLFWwindow* m_Window;
		struct WindowData
		{
			char* Title;
			u32 Width, Height;
		};

		WindowData m_Data;

	};
}
