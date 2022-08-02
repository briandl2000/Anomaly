#pragma once

#include "EngineDefines.h"

#include "Game/Game.h"

namespace Game_Content
{
	class Game_Content : public Game
	{
	public:
		Game_Content();

		virtual ~Game_Content() override;

		virtual void Init() override;

		virtual void Update() override;

		virtual void Shutdown() override;
	};
}