#include "anompch.h"
#include "Application.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Game/Game.h"

namespace Anomaly
{
	struct ApplicationState
	{
		Scope<Window> Window;
	};

	static ApplicationState s_State = {};

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationConfig& appConfig)
	{
		ANOM_CORE_INFO("Creating Application...");

		ANOM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		WindowProps props;
		props.Title = appConfig.Title;
		props.Width = appConfig.Width;
		props.Height = appConfig.Height;

		s_State.Window = CreateScope<Window>();
		s_State.Window->Init(props);
		s_State.Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		ANOM_CORE_INFO("Creating Game...");

		m_Game = appConfig.Game;// GetGame();
		m_Game->Init();

		ANOM_CORE_INFO("Game Created.");

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

			m_Game->Update();
		}
		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		// {
		// 	(*--it)->OnEvent(e);
		// 	if (e.Handled)
		// 		break;
		// }
	}

	void Application::ShutDown()
	{
		ANOM_CORE_INFO("Shutting down Application...");

		m_Game->Shutdown();

		s_State.Window->Shutdown();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}
}
