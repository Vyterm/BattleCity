#include "GameEditor.hpp"
#include "GameMap.hpp"
#include "GameCtrl.hpp"
#include "GameFPS.hpp"
#include "vytTimer.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

static const E_4BitColor itemColors[] =
{
	E_4BitColor::White,
	E_4BitColor::LRed,
	E_4BitColor::LYellow,
	E_4BitColor::LGreen,
	E_4BitColor::LCyan,
	E_4BitColor::LPurple,
	E_4BitColor::LBlue,
	E_4BitColor::LWhite,
};

static E_StaticCellType items[] =
{
	E_StaticCellType::OpenSpace,
	E_StaticCellType::JebelLand,
	E_StaticCellType::GrassLand,
	E_StaticCellType::MagmaLand,
	E_StaticCellType::FrostLand,
	E_StaticCellType::EarthWall,

	E_StaticCellType::GermPoint,
};

static E_4BitColor cellColors[] =
{
	 E_4BitColor::Black,
	 DEFAULT_BACK_COLOR,
	 E_4BitColor::LGreen,
	 E_4BitColor::LRed,
	 E_4BitColor::LBlue,
	 DEFAULT_BACK_COLOR,
	 DEFAULT_BACK_COLOR,
};

static const string itemNames[] =
{
	"空地",
	"山峰",
	"草丛",
	"岩浆",
	"冰块",
	"土墙",
	"出生点",
	"左画右擦",
	"中空填充",
	"完整填充",
	"白色",
	"红色",
	"黄色",
	"绿色",
	"青色",
	"紫色",
	"蓝色",
	"亮白",
	"加载地图",
	"保存地图",
};

#pragma region Editor Painter

void EditorPainter::set_Type(E_EditType type)
{
	m_pointSet.Clear(m_model);
	m_type = type;
}

void EditorPainter::set_ForeColor(E_4BitColor foreColor)
{
	if (m_pointSet.isValid)
		m_model.SetType(m_pointSet.position, m_cellType, foreColor);
	m_foreColor = foreColor;
}

void EditorPainter::set_CellType(E_StaticCellType cellType)
{
	m_pointSet.Clear(m_model);
	m_cellType = cellType;
}

bool EditorPainter::IsDoublePoint() const
{
	return m_type != E_EditType::PenEraser;
}

bool EditorPainter::DrawEditLeftKey(Vector2 &position)
{
	bool isDoublePoint = IsDoublePoint();
	if (isDoublePoint)
	{
		if (m_pointSet.isValid)
		{
			m_pointSet.isValid = false;
			Vector2 minPos = { m_pointSet.position.x < position.x ? m_pointSet.position.x : position.x, m_pointSet.position.y < position.y ? m_pointSet.position.y : position.y };
			Vector2 maxPos = { m_pointSet.position.x > position.x ? m_pointSet.position.x : position.x, m_pointSet.position.y > position.y ? m_pointSet.position.y : position.y };
			if (m_cellType == E_StaticCellType::GermPoint)
				m_model.SetType(position, m_cellType, m_foreColor);
			else if (m_type == E_EditType::HollowSet)
				m_model.SetHollowLand(minPos, maxPos, m_cellType, m_foreColor);
			else if (m_type == E_EditType::CloseySet)
				m_model.SetCloseyLand(minPos, maxPos, m_cellType, m_foreColor);
			else
				return false;
			return true;
		}
		else
		{
			m_pointSet.isValid = true;
			m_pointSet.position = position;
		}
	}
	m_model.SetType(position, m_cellType, m_foreColor);
	return true;
}

bool EditorPainter::DrawEditRightKey(Vector2 &position)
{
	if (!m_pointSet.Clear(m_model))
	{
		m_model.SetType(position, E_StaticCellType::OpenSpace, m_foreColor);
	}
	return true;
}

bool EditorPainter::DrawEdit(Vector2 position, E_EditMode mode)
{
	if (mode == E_EditMode::LeftKey)
		return DrawEditLeftKey(position);
	else if (mode == E_EditMode::RightKey)
		return DrawEditRightKey(position);
	return false;
}

bool PointSet::Clear(GameMapModel & model)
{
	if (!isValid) return false;

	isValid = false;
	model.SetType(position, E_StaticCellType::OpenSpace, DEFAULT_FORE_COLOR);
	return true;

}

#pragma endregion

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
		if (stcRecord.EventType == KEY_EVENT)
			breakFlag &= KeyEventProc(stcRecord.Event.KeyEvent);
		else if (stcRecord.EventType == MOUSE_EVENT)
			breakFlag &= MouseEventProc(stcRecord.Event.MouseEvent);
		m_recordQueue.pop();
	}
	return breakFlag;
}

bool GameEditor::KeyEventProc(KEY_EVENT_RECORD ker)
{
	// 避免缓存的Escape弹起消息导致再次退出，因此按下时不退出
	if (ker.bKeyDown) return true;
	if (ker.wVirtualKeyCode == VK_ESCAPE) return false;
	return true;
}

inline void TryUpdatePainter(const MOUSE_EVENT_RECORD &mer, GameEditor &editor)
{
	if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 116 && mer.dwMousePosition.Y < 6)
	{
		int type = (mer.dwMousePosition.X/2 - 42) / 4 + (mer.dwMousePosition.Y / 4) * 4;
		editor.get_Painter().set_CellType(type > 6 ? editor.get_Painter().get_CellType() : E_StaticCellType(type));
	}
	else if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 114 && mer.dwMousePosition.Y == 7)
	{
		int type = (mer.dwMousePosition.X/2 - 42) / 5;
		editor.get_Painter().set_Type(type > 3 ? editor.get_Painter().get_Type() : E_EditType(type));
	}
	else if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 116 && mer.dwMousePosition.Y > 8 && mer.dwMousePosition.Y <= 12)
	{
		int index = (mer.dwMousePosition.X / 2 - 42) / 4 + ((mer.dwMousePosition.Y - 8) / 3) * 4;
		editor.get_Painter().set_ForeColor(itemColors[index]);
	}
	else if (mer.dwMousePosition.X >= 86 && mer.dwMousePosition.X < 94 && mer.dwMousePosition.Y == 17)
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			editor.Load();
	}
	else if (mer.dwMousePosition.X >= 104 && mer.dwMousePosition.X < 112 && mer.dwMousePosition.Y == 17)
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			editor.Save();
	}
}

inline void TryPaint(const MOUSE_EVENT_RECORD &mer, EditorPainter &painter)
{
	Vector2 position = { mer.dwMousePosition.X / 2, mer.dwMousePosition.Y };
	if ((position.x >= 1 && position.x <= 38 && position.y >= 1 && position.y <= 38) ||
		(position.x >= 41 && position.x <= 58 && position.y >= 21 && position.y <= 38))
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			painter.DrawEdit(position, E_EditMode::LeftKey);
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			painter.DrawEdit(position, E_EditMode::RightKey);
	}
}

bool GameEditor::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
	SetPosition(41, 18);
	switch (mer.dwEventFlags)
	{
		break;
	case DOUBLE_CLICK:
		cout << "鼠标双击" << endl;
		break;
	case 0:
		TryUpdatePainter(mer, *this);
		if (m_painter.IsDoublePoint())
			TryPaint(mer, m_painter);
	case MOUSE_MOVED:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			if (!m_painter.IsDoublePoint())
				TryPaint(mer, m_painter);
			cout << "MM鼠标单击";
		}
		cout << "X:" << setw(2) << setfill('0') << (mer.dwMousePosition.X/2) << "  Y:" << setw(2) << setfill('0') << mer.dwMousePosition.Y << endl;
		break;
	default:
		break;
	}
	return true;
}

#pragma endregion

#pragma region Editor Refresh

//class HighLightTimer : public vyt::timer::handler
//{
//	GameEditor &m_editor;
//public:
//	HighLightTimer(GameEditor &editor) : vyt::timer::handler(100, true), m_editor(editor) { }
//	void Invoke() { m_editor.Refresh(); }
//};

void GameEditor::Refresh()
{
	// ToDo: With timer, but ReadConsoleInput it's blocking mode, and can't use multi-thread, so doesn't implement.
	//static int invokeCount = 0;
	//static bool isHighLight = false;
	//isHighLight = ++invokeCount % 5 == 0;
	static bool isHighLight = true;
	E_4BitColor selectForeColor = m_painter.get_ForeColor();

	E_StaticCellType selectType = m_painter.get_CellType();
	E_EditType selectEditType = m_painter.get_Type();
	int startX = 42, offset = 4, startY = 2;
	for (int i = 0; i < 7; ++i)
	{
		ConsoleColor textColor = int(selectType) == i ? ConsoleColor({ E_4BitColor::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		game::RenderLayer::getInstance().SetString({ startX + offset * (i%4),startY + (i / 4) * 2 }, itemNames[i], game::ToRealColor(textColor.fore), game::ToRealColor(textColor.back));
		auto foreColor = (items[i] == E_StaticCellType::GermPoint || items[i] == E_StaticCellType::EarthWall) ? game::ToRealColor(selectForeColor) : game::ToRealColor(DEFAULT_FORE_COLOR);
		game::RenderLayer::getInstance().SetString({ startX + offset * (i%4),startY + (i / 4) * 2 + 1 }, StaticCellImages[int(items[i])], foreColor, game::ToRealColor(cellColors[i]));
	}

	startX = 42, offset = 5, startY = 7;
	for (int i = 0; i < 3; ++i)
	{
		ConsoleColor textColor = int(selectEditType) == i ? ConsoleColor({ E_4BitColor::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		game::RenderLayer::getInstance().SetString({ startX + offset * (i%4),startY + (i / 4) * 2 }, itemNames[i + 7], game::ToRealColor(textColor.fore), game::ToRealColor(textColor.back));
	}

	startX = 42, offset = 4, startY = 9;
	for (int i = 0; i < 8; ++i)
	{
		ConsoleColor textColor = selectForeColor == itemColors[i] ? ConsoleColor({ E_4BitColor::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 }, itemNames[i + 10], game::ToRealColor(textColor.fore), game::ToRealColor(textColor.back));
		game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 + 1 }, "      ", game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(itemColors[i]));
	}

	startX = 43, offset = 9, startY = 17;
	for (int i = 0; i < 2; ++i)
	{
		ConsoleColor textColor = DEFAULT_COLOR;
		game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 }, itemNames[i + 18], game::ToRealColor(textColor.fore), game::ToRealColor(textColor.back));
	}

	SetColor(DEFAULT_COLOR);
}

#pragma endregion

#pragma region Editor Main

GameEditor::GameEditor() : m_painter(m_mapModel)
{
	New();
	m_hStdin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(m_hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}

void GameEditor::Run()
{
	SetColor(DEFAULT_COLOR);

	DWORD dwMode;
	GetConsoleMode(m_hStdin, &dwMode);
	game::RenderLayer::getInstance().Clear();// This method will call system("cls"), and it will reset console mode
	//SetConsoleMode(m_hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	SetConsoleMode(m_hStdin, dwMode);

	GameMap map;
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
		map.LoadStaticModel(m_mapModel);
		Refresh();
		game::RenderLayer::getInstance().Draw();
	} while (HandleRecord());
}

void GameEditor::New()
{
	m_mapModel.Clear();
	m_mapModel.SetHollowLand({ 0, 0 }, { (GAME_WIDTH + 0 - 1), (GAME_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand, DEFAULT_FORE_COLOR);
	m_mapModel.SetHollowLand({ GAME_WIDTH, MSG_HEIGHT }, { (MAZE_WIDTH + GAME_WIDTH - 1), (MAZE_HEIGHT + MSG_HEIGHT - 1) }, E_StaticCellType::JebelLand, DEFAULT_FORE_COLOR);
	m_mapModel.SetHollowLand({ GAME_WIDTH, 0 }, { (MSG_WIDTH + GAME_WIDTH - 1), (MSG_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand, DEFAULT_FORE_COLOR);
}

void GameEditor::Load()
{
	string path = OpenFile();
	if ("" == path) return;
	m_mapModel.Clear();
	std::ifstream ifs;
	ifs.open(path);
	ifs >> m_mapModel;
	ifs.close();
}

void GameEditor::Save()
{
	string path = SaveFile();
	std::ofstream ofs;
	ofs.open(path);
	ofs << m_mapModel;
	ofs.close();
}

#pragma endregion
