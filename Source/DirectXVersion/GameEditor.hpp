#ifndef GAME_EDITOR_HPP_INCLUDED
#define GAME_EDITOR_HPP_INCLUDED

#include "EditorPainter.hpp"
#include <map>
#include <queue>

class GameEditor
{
	HANDLE m_hStdin;
	EditorPainter m_painter;

	std::queue<INPUT_RECORD> m_recordQueue;
	void ObtainRecord();
	bool HandleRecord();
public:
	EditorPainter &get_Painter() { return m_painter; }
public:
	GameEditor();
	void Run();
};

#endif