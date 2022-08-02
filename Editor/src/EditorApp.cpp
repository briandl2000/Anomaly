#include <Anomaly.h>
#include <Core/EntryPoint.h>

#include "Game/Game.h"


class EditorApp : public Anomaly::Application
{
public:
	EditorApp(const Anomaly::ApplicationConfig& appConfig)
		: Anomaly::Application(appConfig)
	{
		ANOM_INFO("Creating Editor App...");
		ANOM_INFO("Finished creating Editor App");
	}

	~EditorApp()
	{
	}
};

Anomaly::Application* Anomaly::CreateApplication()
{
	Anomaly::ApplicationConfig config;
	config.Width = 900;
	config.Height = 600;
	config.Title = "EditorApp";
	config.Game = CreateGame();

	return new EditorApp(config);
}
