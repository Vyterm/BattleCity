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
	SetTitle(GAME_NAME);
	SetConsoleWindowSize();
	ResetCursor();
}

void GameApp::Run()
{
	pHome = new HomeSurface(true);
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
		Game();
		return true;
	case HomeSurface::Continue:
		UnfinishedSurface(23, 20, 300, "继续游戏尚未完成，敬请期待");
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

void GameApp::Game()
{
	GameMap map;

	LevelModel reloadModel;
	string path = OpenFile();
	if ("" == path) return;
	std::ifstream ifs;
	ifs.open(path);
	ifs >> reloadModel;
	ifs.close();

	map.SetModel(reloadModel);
	char c = '\0';
	while ('q' != c)
	{
		GameMain(map);
		while (_kbhit())
			_getch();
		c = '\0';
		while ('q' != c && 'r' != c)
			c = _getch();
	}
}

void GameApp::Editor()
{
	GameEditor editor;
	editor.Run();
}

void GameApp::GameMain(GameMap & map)
{
	map.Reset();
	bool isGameOver = false;
	bool isGamePause = false;
#ifdef _DEBUG
	// 显示FPS(Frame per second)
	game::FPS fps;
	while (!isGameOver)
	{
		fps.Frame();
#else
	while (!isGameOver)
	{
#endif
		ShowMsg(map.GetPlayer(0), map.GetPlayer(1));
		if (IsKeyDown(VK_SPACE))
			isGamePause = !isGamePause;

		if (isGamePause)
			continue;
		vyt::timer::get_instance().HandleClock();
		isGameOver = map.CheckOver();
		game::RenderLayer::getInstance().Draw();
	}
}
