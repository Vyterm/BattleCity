#include "EditorPainter.hpp"
#include <sstream>
#include <iomanip>

using namespace std;

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

EditorPainter::EditorPainter() : game::Renderer(MSG_WIDTH, MSG_HEIGHT, game::RenderType::UICanvas), m_position(GAME_WIDTH, 0),
	m_type(E_EditType::PenEraser), m_cellType(E_StaticCellType::OpenSpace), m_pointSet({ 0,0 }, false)
{
	m_cellColors[E_StaticCellType::JebelLand] = { E_4BitColor::LWhite,	E_4BitColor::White	};
	m_cellColors[E_StaticCellType::GrassLand] = { E_4BitColor::Black,	E_4BitColor::Green	};
	m_cellColors[E_StaticCellType::MagmaLand] = { E_4BitColor::LRed,	E_4BitColor::Red	};
	m_cellColors[E_StaticCellType::FrostLand] = { E_4BitColor::LCyan,	E_4BitColor::Blue	};
	m_cellColors[E_StaticCellType::EarthWall] = { E_4BitColor::LRed,	E_4BitColor::Yellow	};
	New();
	const string &cellString = StaticCellImages[int(E_StaticCellType::JebelLand)];
	int posXS = 0; int posXE = 19; int posYS = 0; int posYE = 39;
	for (int ci = posXS; ci <= posXE; ++ci)
		CacheString(ci, posYS, cellString);
		//game::RenderLayer::getInstance().SetString({ ci,posYS }, cellString, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
	for (int ci = posXS; ci <= posXE; ++ci)
		CacheString(ci, posYE, cellString);
		//game::RenderLayer::getInstance().SetString({ ci,posYE }, cellString, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
	for (int ri = posYS + 1; ri < posYE; ++ri)
	{
		CacheString(posXS, ri, cellString);
		CacheString(posXE, ri, cellString);
		//game::RenderLayer::getInstance().SetString({ posXS,ri }, cellString, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
		//game::RenderLayer::getInstance().SetString({ posXE,ri }, cellString, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
	}
}

void EditorPainter::set_Type(E_EditType type)
{
	m_pointSet.Clear(m_model);
	m_type = type;
}

void EditorPainter::set_ForeColor(E_4BitColor foreColor)
{
	m_cellColors[m_cellType].fore = foreColor;
	if (m_pointSet.isValid)
		m_model.SetType(m_pointSet.position, m_cellType, m_cellColors[m_cellType]);
}

void EditorPainter::set_BackColor(E_4BitColor backColor)
{
	m_cellColors[m_cellType].back = backColor;
	if (m_pointSet.isValid)
		m_model.SetType(m_pointSet.position, m_cellType, m_cellColors[m_cellType]);
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
				m_model.SetType(position, m_cellType, m_cellColors[m_cellType]);
			else if (m_type == E_EditType::HollowSet)
				m_model.SetHollowLand(minPos, maxPos, m_cellType, m_cellColors[m_cellType]);
			else if (m_type == E_EditType::CloseySet)
				m_model.SetCloseyLand(minPos, maxPos, m_cellType, m_cellColors[m_cellType]);
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
	m_model.SetType(position, m_cellType, m_cellColors[m_cellType]);
	return true;
}

bool EditorPainter::DrawEditRightKey(Vector2 &position)
{
	if (!m_pointSet.Clear(m_model))
	{
		m_model.SetType(position, E_StaticCellType::OpenSpace, m_cellColors[m_cellType]);
	}
	return true;
}

bool EditorPainter::KeyEventProc(KEY_EVENT_RECORD ker)
{
	// 避免缓存的Escape弹起消息导致再次退出，因此按下时不退出
	if (ker.bKeyDown) return true;
	if (ker.wVirtualKeyCode == VK_ESCAPE)
		return false;
	return true;
}

inline void TryUpdatePainter(const MOUSE_EVENT_RECORD &mer, EditorPainter &editor)
{
	if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 116 && mer.dwMousePosition.Y < 6)
	{
		int type = (mer.dwMousePosition.X / 2 - 42) / 4 + (mer.dwMousePosition.Y / 4) * 4;
		editor.set_CellType(type > 6 ? editor.get_CellType() : E_StaticCellType(type));
	}
	else if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 114 && mer.dwMousePosition.Y == 7)
	{
		int type = (mer.dwMousePosition.X / 2 - 42) / 5;
		editor.set_Type(type > 3 ? editor.get_Type() : E_EditType(type));
	}
	else if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 116 && mer.dwMousePosition.Y > 8 && mer.dwMousePosition.Y <= 12)
	{
		int index = (mer.dwMousePosition.X / 2 - 42) / 4 + ((mer.dwMousePosition.Y - 8) / 3) * 4;
		editor.set_ForeColor(itemColors[index]);
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
	if ((position.x >= 1 && position.x <= 39 && position.y >= 1 && position.y <= 38))
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			painter.DrawEdit(position, E_EditMode::LeftKey);
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			painter.DrawEdit(position, E_EditMode::RightKey);
	}
}

bool EditorPainter::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
	int mouseLogX = 1;
	int mouseLogY = 38;
	ostringstream oss;
	switch (mer.dwEventFlags)
	{
		break;
	case DOUBLE_CLICK:
		oss << "鼠标双击";
		CacheString(mouseLogX, mouseLogY, "鼠标双击");
		break;
	case 0:
		TryUpdatePainter(mer, *this);
		if (IsDoublePoint())
			TryPaint(mer, *this);
	case MOUSE_MOVED:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			if (!IsDoublePoint())
				TryPaint(mer, *this);
			oss << "MM鼠标单击";
		}
		oss << "X:" << setw(2) << setfill('0') << (mer.dwMousePosition.X / 2) << "  Y:" << setw(2) << setfill('0') << mer.dwMousePosition.Y << endl;
		break;
	default:
		break;
	}
	CacheString(mouseLogX, mouseLogY, oss.str());
	return true;
}

void EditorPainter::New()
{
	m_model.Clear();
	m_model.SetHollowLand({ 0, 0 }, { (GAME_WIDTH + 0 - 1), (GAME_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand, DEFAULT_COLOR);
}

void EditorPainter::Load()
{
	string path = OpenFile();
	if ("" == path) return;
	m_model.Clear();
	std::ifstream ifs;
	ifs.open(path);
	ifs >> m_model;
	ifs.close();
}

void EditorPainter::Save()
{
	string path = SaveFile();
	std::ofstream ofs;
	ofs.open(path);
	ofs << m_model;
	ofs.close();
}

bool EditorPainter::DrawEdit(Vector2 position, E_EditMode mode)
{
	if (mode == E_EditMode::LeftKey)
		return DrawEditLeftKey(position);
	else if (mode == E_EditMode::RightKey)
		return DrawEditRightKey(position);
	return false;
}

void EditorPainter::Rerender()
{
	E_4BitColor selectForeColor = get_ForeColor();

	E_StaticCellType selectType = get_CellType();
	E_EditType selectEditType = get_Type();
	int startX = 2, offset = 4, startY = 2;
	for (int i = 0; i < 7; ++i)
	{
		ConsoleColor textColor = int(selectType) == i ? ConsoleColor({ E_4BitColor::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		CacheString(startX + offset * (i % 4), startY + (i / 4) * 2, itemNames[i], textColor);
		//game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 }, itemNames[i], game::ToRealColor(textColor.fore), game::ToRealColor(textColor.back));
		CacheString(startX + offset * (i % 4), startY + (i / 4) * 2 + 1, StaticCellImages[int(items[i])], m_cellColors[items[i]]);
		//game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 + 1 },
		//	// ToDo: Draw with painter color
		//	//StaticCellImages[int(items[i])], game::ToRealColor(m_painter.get_ForeColor()), game::ToRealColor(m_painter.get_BackColor()));
		//	StaticCellImages[int(items[i])], game::ToRealColor(cellForeColors[i]), game::ToRealColor(cellBackColors[i]));
	}

	startX = 2, offset = 5, startY = 7;
	for (int i = 0; i < 3; ++i)
	{
		ConsoleColor textColor = int(selectEditType) == i ? ConsoleColor({ E_4BitColor::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		CacheString(startX + offset * (i % 4), startY + (i / 4) * 2, itemNames[i + 7], textColor);
		//game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 }, itemNames[i + 7], game::ToRealColor(textColor.fore), game::ToRealColor(textColor.back));
	}

	startX = 2, offset = 4, startY = 9;
	for (int i = 0; i < 8; ++i)
	{
		ConsoleColor textColor = selectForeColor == itemColors[i] ? ConsoleColor({ E_4BitColor::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		CacheString(startX + offset * (i % 4), startY + (i / 4) * 2, itemNames[i + 10], textColor);
		CacheString(startX + offset * (i % 4), startY + (i / 4) * 2 + 1, "      ", { DEFAULT_FORE_COLOR, itemColors[i] });
		//game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 }, itemNames[i + 10], game::ToRealColor(textColor.fore), game::ToRealColor(textColor.back));
		//game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 + 1 }, "      ", game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(itemColors[i]));
	}

	startX = 3, offset = 9, startY = 36;
	for (int i = 0; i < 2; ++i)
	{
		ConsoleColor textColor = DEFAULT_COLOR;
		CacheString(startX + offset * (i % 4), startY + (i / 4) * 2, itemNames[i + 18], textColor);
		//game::RenderLayer::getInstance().SetString({ startX + offset * (i % 4),startY + (i / 4) * 2 }, itemNames[i + 18], game::ToRealColor(textColor.fore), game::ToRealColor(textColor.back));
	}

}

bool EditorPainter::Update(const INPUT_RECORD & record)
{
	bool continueFlag = true;
	if (record.EventType == KEY_EVENT)
		continueFlag &= KeyEventProc(record.Event.KeyEvent);
	else if (record.EventType == MOUSE_EVENT)
		continueFlag &= MouseEventProc(record.Event.MouseEvent);
	Rerender();
	map.LoadStaticModel(m_model);
	return continueFlag;
}

bool PointSet::Clear(LevelModel & model)
{
	if (!isValid) return false;

	isValid = false;
	model.SetType(position, E_StaticCellType::OpenSpace, DEFAULT_COLOR);
	return true;

}
