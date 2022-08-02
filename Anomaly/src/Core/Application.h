#pragma once

#include "EngineDefines.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"

class Game;

namespace Anomaly {

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	struct ApplicationConfig
	{
		char* Title;
		u32 Width;
		u32 Height;

		Anomaly::Ref<Game> Game;
	};

	class Application
	{
	public:
		ANOM_API Application(const ApplicationConfig& appConfig);

		ANOM_API virtual ~Application();

		ANOM_API void Run();
		
		inline static Application& Get() { return *s_Instance; }

		void OnEvent(Event& e);

	private:
		void ShutDown();

		bool OnWindowClose(WindowCloseEvent& e);

	private:
		b8 m_Running = TRUE;
		Ref<Game> m_Game;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
