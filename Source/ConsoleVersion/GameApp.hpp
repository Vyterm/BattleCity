#pragma once

#include "GameMap.hpp"
#include "TankController.hpp"

class GameApp
{
	void GameMain(GameMap &map);
public:
	GameApp();
	void Run();
	bool Home();
	void Game();
	void Editor();
};

int main();