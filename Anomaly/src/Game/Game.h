#pragma once

#include "Anomaly.h"
#include "EngineDefines.h"

class Game
{
public:
	Game() {};
	virtual ~Game(){};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
};

#ifndef ANOM_ENGINE
GAME_API Anomaly::Ref<Game> CreateGame();
#endif
