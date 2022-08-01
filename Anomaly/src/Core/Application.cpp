#include "anompch.h"
#include "Application.h"

namespace Anomaly
{
	struct ApplicationState
	{
		std::unique_ptr<Window> Window;
	};

	static ApplicationState s_State = {};

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationConfig& appConfig)
	{
		Anomaly::InitializaLogging();
		ANOM_CORE_INFO("Creating Application...");

		ANOM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		WindowProps props;
		props.Title = appConfig.Title;
		props.Width = appConfig.Width;
		props.Height = appConfig.Height;


		s_State.Window = std::unique_ptr<Window>(new Window(props));

		ANOM_CORE_INFO("Application Created.");

	}

	Application::~Application()
	{
		ShutDown();
	}


	void Application::Run()
	{
		ANOM_CORE_INFO("Starting Main Loop...");

		while(m_Running)
		{
			s_State.Window->OnUpdate();
		}
		
	}

	void Application::ShutDown()
	{
		ANOM_CORE_INFO("Shutting down Application...");

		s_State.Window = nullptr;

		ShutdownLogging();
	}
}
