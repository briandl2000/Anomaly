#pragma once

#include "EngineDefines.h"

#include "Application.h"

extern Anomaly::Application* CreateApplication();

int main(int argc, char** argv)
{
	Anomaly::Application* app = CreateApplication();
	app->Run();
	delete app;

	return 0; 
}