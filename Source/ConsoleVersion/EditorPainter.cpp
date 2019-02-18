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
	"�յ�",
	"ɽ��",
	"�ݴ�",
	"�ҽ�",
	"����",
	"��ǽ",
	"������",
	"���Ҳ�",
	"�п����",
	"�������",
	"��ɫ",
	"����",
	"����",
	"����",
	"����",
	"����",
	"����",
	"��ɫ",
	"��ɫ",
	"��ɫ",
	"��ɫ",
	"��ɫ",
	"��ɫ",
	"��ɫ",
	"��ɫ",
	"����",
	"����ǰ��ɫ",
	"���ñ���ɫ",
	"�½���ͼ",
	"���ص�ͼ",
	"�����ͼ",
};

inline int toX(int preIndex, int index, int offset, size_t countPerLine)
{
	return preIndex + offset * (index % countPerLine);
}

inline int toY(int preIndex, int index, int yOffset, size_t countPerLine)
{
	return preIndex + yOffset * (index / countPerLine);
}

//#define CPL(sp,xo) (MSG_WIDTH-sp)/xo;
inline constexpr int CPL(int sp, int xo) { return (MSG_WIDTH - sp) / xo; }
inline constexpr int LineCount(int count, int cpl) { return  (count / cpl) + (count % cpl == 0 ? 0 : 1); }
inline constexpr int YOffset(int baseY, int yOffset, int lc) { return (baseY + 1) + lc*yOffset; }

constexpr auto BrushX = 2;
constexpr auto BrushY = 2;
constexpr auto BrushXOffset = 4;
constexpr auto BrushYOffset = 2;
constexpr auto BrushCount = 7;
constexpr auto BrushCountPerLine = CPL(BrushX, BrushXOffset);
constexpr auto BrushLineCount = LineCount(BrushCount, BrushCountPerLine);

constexpr auto BModeX = 2;
constexpr auto BModeY = YOffset(BrushY, BrushYOffset, BrushLineCount);
constexpr auto BModeXOffset = 5;
constexpr auto BModeYOffset = 1;
constexpr auto BModeCount = 3;
constexpr auto BModeCountPerLine = CPL(BModeX, BModeXOffset);
constexpr auto BModeLineCount = LineCount(BModeCount, BModeCountPerLine);

constexpr auto ColorX = 2;
constexpr auto ColorY = YOffset(BModeY, BModeYOffset, BModeLineCount);
constexpr auto ColorXOffset = 4;
constexpr auto ColorYOffset = 2;
constexpr auto ColorCount = 16;
constexpr auto ColorCountPerLine = CPL(ColorX, ColorXOffset);
constexpr auto ColorLineCount = LineCount(ColorCount, ColorCountPerLine);

constexpr auto CModeX = 3;
constexpr auto CModeY = YOffset(ColorY, ColorYOffset, ColorLineCount);
constexpr auto CModeXOffset = 8;
constexpr auto CModeYOffset = 1;
constexpr auto CModeCount = 2;
constexpr auto CModeCountPerLine = CPL(CModeX, CModeXOffset);
constexpr auto CModeLineCount = LineCount(CModeCount, CModeCountPerLine);

constexpr auto OptionX = 2;
constexpr auto OptionY = 36;
constexpr auto OptionXOffset = 5;
constexpr auto OptionYOffset = 1;
constexpr auto OptionCount = 3;
constexpr auto OptionCountPerLine = CPL(OptionX, OptionXOffset);
constexpr auto OptionLineCount = LineCount(OptionCount, OptionCountPerLine);

EditorPainter::EditorPainter() : game::Renderer(MSG_WIDTH, MSG_HEIGHT, game::RenderType::UICanvas), m_position(GAME_WIDTH, 0),
	m_type(E_EditType::PenEraser), m_colorType(E_ColorType::SetForeColor), m_cellType(E_StaticCellType::OpenSpace), m_pointSet({ 0,0 }, false)
{
	m_cellColors[E_StaticCellType::JebelLand] = { E_4BitColor::LWhite,	E_4BitColor::White	};
	m_cellColors[E_StaticCellType::GrassLand] = { E_4BitColor::Black,	E_4BitColor::Green	};
	m_cellColors[E_StaticCellType::MagmaLand] = { E_4BitColor::LRed,	E_4BitColor::Red	};
	m_cellColors[E_StaticCellType::FrostLand] = { E_4BitColor::LCyan,	E_4BitColor::Blue	};
	m_cellColors[E_StaticCellType::EarthWall] = { E_4BitColor::LRed,	E_4BitColor::Yellow	};
	New();
	const string &cellString = StaticCellImages[int(E_StaticCellType::JebelLand)];
	int posXS = 0; int posXE = MSG_WIDTH-1; int posYS = 0; int posYE = MSG_HEIGHT-1;
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
	// ���⻺���Escape������Ϣ�����ٴ��˳�����˰���ʱ���˳�
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
		oss << "���˫��";
		CacheString(mouseLogX, mouseLogY, "���˫��");
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
			oss << "MM��굥��";
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
	int mx = mer.dwMousePosition.X / 2 - m_position.x;
	int my = mer.dwMousePosition.Y - m_position.y;
	if (mx > 0 && mx < MSG_WIDTH && my < 6)
	{
		int type = (mx - 1) / BrushXOffset + ((my - BrushY) / BrushYOffset) * BrushCountPerLine;
		set_CellType(type > BrushCount - 1 ? get_CellType() : E_StaticCellType(type));
	}
	else if (mx > 0 && mx < MSG_WIDTH && my == BModeY)
	{
		int type = (mx - 1) / BModeXOffset;
		set_Type(type > BModeCount - 1 ? get_Type() : E_EditType(type));
	}
	else if (mx > 0 && mx < MSG_WIDTH && my >= ColorY && my <= ColorY + ColorLineCount * ColorYOffset - 1)
	{
		int index = (mx - 1) / ColorXOffset + ((my - ColorY) / ColorYOffset) * ColorCountPerLine;
		if (E_ColorType::SetForeColor == m_colorType)
			set_ForeColor(index > ColorCount - 1 ? get_ForeColor() : E_4BitColor(index));
		else
			set_BackColor(index > ColorCount - 1 ? get_BackColor() : E_4BitColor(index));
	}
	else if (mx > 0 && mx < MSG_WIDTH && my == CModeY)
	{
		int type = (mx - 1) / CModeXOffset;
		m_colorType = (type > CModeCount - 1 ? m_colorType : E_ColorType(type));
	}
	else if (my == OptionY && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
	{
		static int newCount = 0, loadCount = 0, saveCount = 0;
		std::ostringstream oss;
		if (mx >= OptionXOffset * 0 + 2 && mx <= OptionXOffset * 1)
			//New();
			oss << "New : " << ++newCount;
		else if (mx >= OptionXOffset * 1 + 2 && mx <= OptionXOffset * 2)
			//Load();
			oss << "Load: " << ++loadCount;
		else if (mx >= OptionXOffset * 2 + 2 && mx <= OptionXOffset * 3)
			//Save();
			oss << "Save: " << ++saveCount;
		CacheString(2, 30, oss.str());
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
	for (int i = 0; i < BrushCount; ++i)
	{
		ConsoleColor textColor = int(get_CellType()) == i ? highLightColor : DEFAULT_COLOR;
		CacheString(toX(BrushX, i, BrushXOffset, BrushCountPerLine), toY(BrushY + 0, i, BrushYOffset, BrushCountPerLine), itemNames[i], textColor);
		CacheString(toX(BrushX, i, BrushXOffset, BrushCountPerLine), toY(BrushY + 1, i, BrushYOffset, BrushCountPerLine), StaticCellImages[int(items[i])], m_cellColors[items[i]]);
	}
	for (int i = 0; i < BModeCount; ++i)
	{
		ConsoleColor textColor = int(get_Type()) == i ? highLightColor : DEFAULT_COLOR;
		CacheString(toX(BModeX, i, BModeXOffset, BModeCountPerLine), toY(BModeY, i, BModeYOffset, BModeCountPerLine), itemNames[i + BrushCount], textColor);
	}
	for (int i = 0; i < ColorCount; ++i)
	{
		ConsoleColor textColor = (E_ColorType::SetForeColor == m_colorType ? get_ForeColor() : get_BackColor()) == E_4BitColor(i) ? highLightColor : DEFAULT_COLOR;
		CacheString(toX(ColorX, i, ColorXOffset, ColorCountPerLine), toY(ColorY + 0, i, ColorYOffset, ColorCountPerLine), itemNames[i + BrushCount + BModeCount], textColor);
		CacheString(toX(ColorX, i, ColorXOffset, ColorCountPerLine), toY(ColorY + 1, i, ColorYOffset, ColorCountPerLine), "      ", { DEFAULT_FORE_COLOR, E_4BitColor(i) });
	}
	for (int i = 0; i < CModeCount; ++i)
	{
		ConsoleColor textColor = int(m_colorType) == i ? highLightColor : DEFAULT_COLOR;
		CacheString(toX(CModeX, i, CModeXOffset, CModeCountPerLine), toY(CModeY, i, CModeYOffset, CModeCountPerLine), itemNames[i + BrushCount + BModeCount + ColorCount], textColor);
	}
	for (int i = 0; i < OptionCount; ++i)
	{
		CacheString(toX(OptionX, i, OptionXOffset, OptionCountPerLine), toY(OptionY, i, OptionYOffset, OptionCountPerLine), itemNames[i + BrushCount + BModeCount + ColorCount + CModeCount]);
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
