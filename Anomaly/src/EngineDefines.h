#pragma once

#if ANOM_BUILD_DLL
#define ANOM_API __declspec(dllexport)
#else
#define ANOM_API __declspec(dllimport)
#endif