#pragma once

#include "EngineDefines.h"

#include "Application.h"

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif

extern Anomaly::Application* CreateApplication(int argc, char** argv);

namespace Anomaly
{
	int Main(int argc, char** argv)
	{
		Anomaly::Application* app = CreateApplication(argc, argv);
		app->Run();
		delete app;

		return 0; 
	}
}
#ifdef RELEASE
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Anomaly::Main(__argc, __argv);
}
#else
int main(int argc, char** argv)
{
	return Anomaly::Main(argc, argv);
}
#endif