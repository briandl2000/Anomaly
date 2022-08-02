#include "Game_Content.h"


namespace Game_Content
{
	Game_Content::Game_Content()
	{
	}

	Game_Content::~Game_Content()
	{
	}

	void Game_Content::Init()
	{
		ANOM_INFO("Game_Content Init");
	}

	void Game_Content::Update()
	{
		//ANOM_DEBUG("Game_Content Update");
	}

	void Game_Content::Shutdown()
	{
	}
	
}


Anomaly::Ref<Game> CreateGame()
{
	return Anomaly::CreateRef<Game_Content::Game_Content>();
}