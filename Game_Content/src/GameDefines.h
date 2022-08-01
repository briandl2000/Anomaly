#pragma once

#include "EngineDefines.h"

#if GAME_BUILD_DLL
#define GAME_API __declspec(dllexport)
#else
#define GAME_API __declspec(dllimport)
#endif

GAME_API void HelloGame();