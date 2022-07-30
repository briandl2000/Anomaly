#include <iostream>

#include "EngineDefines.h"
#include "Core/Log.h"
#include "GameDefines.h"


int main()
{
	Anomaly::InitializaLogging();

	ANOM_ERROR("This is a test: %f", 0.324234f);
	ANOM_WARN("This is a test: %f", 0.324234f);
	ANOM_INFO("This is a test: %f", 0.324234f);
	ANOM_DEBUG("This is a test: %f", 0.324234f);
	ANOM_TRACE("This is a test: %f", 0.324234f);

	std::cout << "Hello Editor!" << std::endl;

	return 0;
}
