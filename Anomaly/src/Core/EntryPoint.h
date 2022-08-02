#pragma once

#include "EngineDefines.h"

extern Anomaly::Application* Anomaly::CreateApplication();

int main(int argc, char** argv)
{
	Anomaly::InitializaLogging();

	auto app = Anomaly::CreateApplication();
	app->Run();
	delete app;

	Anomaly::ShutdownLogging();

	return 0; 
}