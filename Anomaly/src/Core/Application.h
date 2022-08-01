#pragma once

#include "EngineDefines.h"
#include "Window.h"

namespace Anomaly {

	struct ApplicationConfig
	{
		char* Title;
		u32 Width;
		u32 Height;
	};

	class Application
	{
	public:
		ANOM_API Application(const ApplicationConfig& appConfig);

		ANOM_API virtual ~Application();

		ANOM_API void Run();
		
		inline static Application& Get() { return *s_Instance; }

	private:
		void ShutDown();

	private:
		b8 m_Running = TRUE;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
