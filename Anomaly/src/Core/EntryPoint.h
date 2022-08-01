#pragma once

#include "EngineDefines.h"

extern Anomaly::Application* Anomaly::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Anomaly::CreateApplication();
	app->Run();
	delete app;
}