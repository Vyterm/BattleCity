#pragma once

#include "GameMap.hpp"
#include "TankController.hpp"

class GameApp
{
	void GameLoop(GameMap &map);
	// While return value is true, direct break loop
	bool GameMain(GameMap &map);
public:
	GameApp();
	void Run();
	bool Home();
	void NewGame();
	void Resume();
	void Editor();
};

int main();