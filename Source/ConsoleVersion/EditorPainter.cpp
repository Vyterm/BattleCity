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
	"黑色",
	"暗蓝",
	"暗绿",
	"暗青",
	"暗红",
	"暗紫",
	"暗黄",
	"白色",
	"灰色",
	"蓝色",
	"绿色",
	"青色",
	"红色",
	"紫色",
	"黄色",
	"亮白",
	"加载地图",
	"保存地图",
};

inline int toX(int preIndex, int index, int offset, size_t countPerLine)
{
	return preIndex + offset * (index % countPerLine);
}

inline int toY(int preIndex, int index, int yOffset, size_t countPerLine)
{
	return preIndex + yOffset * (index / countPerLine);
}

constexpr auto BrushY = 2;
constexpr auto BModeY = 7;
constexpr auto ColorY = 9;
constexpr auto OptionY = 36;

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
	for (int ci = posXS; ci <= posXE; ++ci)
		CacheString(ci, posYE, cellString);
	for (int ri = posYS + 1; ri < posYE; ++ri)
	{
		CacheString(posXS, ri, cellString);
		CacheString(posXE, ri, cellString);
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
		TryUpdatePainter(mer);
		if (IsDoublePoint())
			TryPaint(mer);
	case MOUSE_MOVED:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			if (!IsDoublePoint())
				TryPaint(mer);
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

void EditorPainter::TryUpdatePainter(const MOUSE_EVENT_RECORD & mer)
{
	if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 116 && mer.dwMousePosition.Y < 6)
	{
		int type = (mer.dwMousePosition.X / 2 - 42) / 4 + (mer.dwMousePosition.Y / 4) * 4;
		set_CellType(type > 6 ? get_CellType() : E_StaticCellType(type));
	}
	else if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 114 && mer.dwMousePosition.Y == BModeY)
	{
		int type = (mer.dwMousePosition.X / 2 - 42) / 5;
		set_Type(type > 3 ? get_Type() : E_EditType(type));
	}
	else if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 116 && mer.dwMousePosition.Y >= ColorY && mer.dwMousePosition.Y <= ColorY + 7)
	{
		int index = (mer.dwMousePosition.X / 2 - 42) / 4 + ((mer.dwMousePosition.Y - ColorY) / 2) * 4;
		set_ForeColor(E_4BitColor(index));
	}
	else if (mer.dwMousePosition.X >= 86 && mer.dwMousePosition.X < 94 && mer.dwMousePosition.Y == OptionY)
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			Load();
	}
	else if (mer.dwMousePosition.X >= 104 && mer.dwMousePosition.X < 112 && mer.dwMousePosition.Y == OptionY)
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			Save();
	}

}

void EditorPainter::TryPaint(const MOUSE_EVENT_RECORD & mer)
{
	Vector2 position = { mer.dwMousePosition.X / 2, mer.dwMousePosition.Y };
	if ((position.x >= 1 && position.x <= 39 && position.y >= 1 && position.y <= 38))
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			DrawEdit(position, E_EditMode::LeftKey);
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			DrawEdit(position, E_EditMode::RightKey);
	}
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
	ConsoleColor highLightColor = { E_4BitColor::LWhite, E_4BitColor::Gray };
	for (int i = 0, countPerLine = 4; i < 7; ++i)
	{
		ConsoleColor textColor = int(get_CellType()) == i ? highLightColor : DEFAULT_COLOR;
		CacheString(toX(2, i, 4, 4), toY(BrushY + 0, i, 2, 4), itemNames[i], textColor);
		CacheString(toX(2, i, 4, 4), toY(BrushY + 1, i, 2, 4), StaticCellImages[int(items[i])], m_cellColors[items[i]]);
	}
	for (int i = 0, countPerLine = 3; i < 3; ++i)
	{
		ConsoleColor textColor = int(get_Type()) == i ? highLightColor : DEFAULT_COLOR;
		CacheString(toX(2, i, 5, countPerLine), toY(BModeY, i, 1, countPerLine), itemNames[i + 7], textColor);
	}
	for (int i = 0, countPerLine = 4; i < 16; ++i)
	{
		ConsoleColor textColor = get_ForeColor() == E_4BitColor(i) ? highLightColor : DEFAULT_COLOR;
		CacheString(toX(2, i, 4, countPerLine), toY(ColorY + 0, i, 2, countPerLine), itemNames[i + 10], textColor);
		CacheString(toX(2, i, 4, countPerLine), toY(ColorY + 1, i, 2, countPerLine), "      ", { DEFAULT_FORE_COLOR, E_4BitColor(i) });
	}
	;
	for (int i = 0, countPerLine = 2; i < 2; ++i)
	{
		ConsoleColor textColor = DEFAULT_COLOR;
		CacheString(toX(3, i, 9, countPerLine), toY(OptionY, i, 1, countPerLine), itemNames[i + 26], textColor);
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
