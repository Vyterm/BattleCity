#include "GameApp.hpp"
#include "GameSurface.hpp"
#include "GamePlayer.hpp"
#include "GameEditor.hpp"
#include "GameGraphic.hpp"
#include "GameFPS.hpp"
#include "vytTimer.hpp"
#include "winapi.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

static LPCSTR BGAudioPath[] =
{
	"sound_bg1.wav",
	"sound_bg2.wav",
	"sound_bg3.wav",
	"sound_bg4.wav",
	"sound_bg5.wav",
};

int main()
{
	GameApp app;
	app.Run();

	return 0;
}

static HomeSurface *pHome;

GameApp::GameApp()
{
	WindowManager::Get()->Initialize(GAME_NAME, WIN_WIDTH, WIN_HEIGHT);
}

void GameApp::Run()
{
	pHome = new HomeSurface(GameMap::ExistArchive());
	PlaySound(TEXT(BGAudioPath[rand() % 5]), NULL, SND_FILENAME | SND_ASYNC);
	while (Home())
		continue;
}

bool GameApp::Home()
{
	pHome->SetActive(true);
	while (pHome->IsActive())
	{
		pHome->Update();
		game::RenderLayer::getInstance().Draw();
	}
	switch (pHome->get_Option())
	{
	case HomeSurface::NewGame:
		NewGame();
		return true;
	case HomeSurface::Continue:
		Resume();
		return true;
	case HomeSurface::Setting:
		UnfinishedSurface(23, 20, 300, "游戏设置尚未完成，敬请期待");
		return true;
	case HomeSurface::Editor:
		Editor();
		return true;
	case HomeSurface::Quit:
	default:
		return false;
	}
}

void GameApp::NewGame()
{
	GameMap map;
	if (map.LoadByBrowse())
		GameLoop(map);
}

void GameApp::Resume()
{
	GameMap map;
	if (map.LoadByArchive())
		GameLoop(map);
}

void GameApp::Editor()
{
	GameEditor editor;
	editor.Run();
}


void GameApp::GameLoop(GameMap & map)
{
	char c = '\0';
	while ('q' != c)
	{
		if (GameMain(map))
			break;
		while (_kbhit())
			_getch();
		c = '\0';
		while ('q' != c && 'r' != c)
			c = _getch();
		if ('q' != c)
			map.Reset();
	}
}

bool GameApp::GameMain(GameMap & map)
{
	bool isGameOver = false;
	bool isGamePause = false;
	// 显示FPS(Frame per second)
	game::FPS fps;
	while (!isGameOver)
	{
		fps.Frame();
		ShowMsg(map, isGamePause);
		isGameOver = map.CheckOver();
		game::RenderLayer::getInstance().Draw();
		if (IsKeyDown(VK_SPACE))
			isGamePause = !isGamePause;

		if (!isGamePause)
			vyt::timer::get_instance().HandleClock();
		else if (IsKeyDown(VK_ESCAPE))
		{
			map.Archive();
			return true;
		}
	}
	return false;
}
