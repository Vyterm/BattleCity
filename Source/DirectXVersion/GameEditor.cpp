#include "GameEditor.hpp"
#include "GameMap.hpp"
#include "TankController.hpp"
#include "GameFPS.hpp"
#include "vytTimer.hpp"
#include "winapi.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

#pragma region Record Proc

void GameEditor::ObtainRecord()
{
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	ReadConsoleInput(m_hStdin, &stcRecord, 1, &dwRead);
	m_recordQueue.push(stcRecord);
}

bool GameEditor::HandleRecord()
{
	bool breakFlag = true;
	while (!m_recordQueue.empty())
	{
		INPUT_RECORD &stcRecord = m_recordQueue.front();
		breakFlag &= m_painter.Update(stcRecord);
		m_recordQueue.pop();
	}
	return breakFlag;
}

#pragma endregion

#pragma region Editor Main

GameEditor::GameEditor()
{
	m_hStdin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(m_hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}

void GameEditor::Run()
{
	DWORD dwMode;
	GetConsoleMode(m_hStdin, &dwMode);
	game::RenderLayer::getInstance().Clear();// This method will call system("cls"), and it will reset console mode
	//SetConsoleMode(m_hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	SetConsoleMode(m_hStdin, dwMode);

#ifdef _DEBUG
	game::FPS fps;
	do
	{
		fps.Frame();
#else
	do
	{
#endif
		ObtainRecord();
		vyt::timer::get_instance().HandleClock();
		game::RenderLayer::getInstance().Draw();
	} while (HandleRecord());
}

#pragma endregion
