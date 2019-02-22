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
	"河流",
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
	"设置前景色",
	"设置背景色",
	"新建地图",
	"加载地图",
	"保存地图",
	"返回上关",
	"创建下关",
	"进入下关",
	"删除下关",
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
inline int CPL(int sp, int xo) { return (MSG_WIDTH - sp) / xo; }
inline int LineCount(int count, int cpl) { return  (count / cpl) + (count % cpl == 0 ? 0 : 1); }
inline int YOffset(int baseY, int yOffset, int lc) { return (baseY + 1) + lc*yOffset; }

const auto BrushX = 2;
const auto BrushY = 2;
const auto BrushXOffset = 4;
const auto BrushYOffset = 2;
const auto BrushCount = 7;
const auto BrushCountPerLine = CPL(BrushX, BrushXOffset);
const auto BrushLineCount = LineCount(BrushCount, BrushCountPerLine);

const auto BModeX = 2;
const auto BModeY = YOffset(BrushY, BrushYOffset, BrushLineCount);
const auto BModeXOffset = 5;
const auto BModeYOffset = 1;
const auto BModeCount = 3;
const auto BModeCountPerLine = CPL(BModeX, BModeXOffset);
const auto BModeLineCount = LineCount(BModeCount, BModeCountPerLine);

const auto ColorX = 2;
const auto ColorY = YOffset(BModeY, BModeYOffset, BModeLineCount);
const auto ColorXOffset = 4;
const auto ColorYOffset = 2;
const auto ColorCount = 16;
const auto ColorCountPerLine = CPL(ColorX, ColorXOffset);
const auto ColorLineCount = LineCount(ColorCount, ColorCountPerLine);

const auto CModeX = 3;
const auto CModeY = YOffset(ColorY, ColorYOffset, ColorLineCount);
const auto CModeXOffset = 8;
const auto CModeYOffset = 1;
const auto CModeCount = 2;
const auto CModeCountPerLine = CPL(CModeX, CModeXOffset);
const auto CModeLineCount = LineCount(CModeCount, CModeCountPerLine);

const auto LevelX = 2;
const auto LevelY = 2 + YOffset(CModeY, CModeYOffset, CModeLineCount);
const auto LevelXOffset = 5;
const auto LevelYOffset = 1;
const auto LevelCount = 3;
const auto LevelCountPerLine = CPL(LevelX, LevelXOffset);
const auto LevelLineCount = LineCount(LevelCount, LevelCountPerLine);

const auto OptionX = 2;
const auto OptionY = 36;
const auto OptionXOffset = 5;
const auto OptionYOffset = 1;
const auto OptionCount = 3;
const auto OptionCountPerLine = CPL(OptionX, OptionXOffset);
const auto OptionLineCount = LineCount(OptionCount, OptionCountPerLine);
ConsoleColor highLightColor = { E_4BitColor::LWhite, E_4BitColor::Gray };

void PainterToogleGroup::RenderOption()
{
	for (enumType i = 0; i < m_items.size(); ++i)
		CacheString(toX(m_position.x, i, m_xOffset, m_itemPerLine), toY(m_position.y, i, m_yOffset, m_itemPerLine), m_items[i]);
}

PainterToogleGroup::PainterToogleGroup(std::vector<std::string>&& itemHints, EventHandler optionChangedHandler,
	int x, int y, int xOffset, enumType * option, OptionMaterial optionMaterial) :
	Renderer(MSG_WIDTH, LineCount(itemHints.size(), CPL(x, xOffset)), game::RenderType::UICanvas),
	m_items(itemHints), m_optionChangedHandler(optionChangedHandler), m_optionMaterial(optionMaterial),
	m_position(x, y), m_drawPosition(GAME_WIDTH, m_position.y),
	m_xOffset(xOffset), m_yOffset(nullptr == m_optionMaterial ? 1 : 2), m_itemPerLine(CPL(x, xOffset)), m_lineCount(LineCount(m_items.size(), m_itemPerLine))
{
	RenderOption();
	RefreshOptionMaterial();
	RefreshOption(option);
}

void PainterToogleGroup::RefreshOption(enumType * option)
{
	if (nullptr != m_option)
		CacheString(toX(m_position.x, *m_option, m_xOffset, m_itemPerLine), toY(m_position.y, *m_option, m_yOffset, m_itemPerLine), m_items[*m_option], DEFAULT_COLOR);
	m_option = option;
	if (nullptr != m_option)
		CacheString(toX(m_position.x, *m_option, m_xOffset, m_itemPerLine), toY(m_position.y, *m_option, m_yOffset, m_itemPerLine), m_items[*m_option], highLightColor);
}

void PainterToogleGroup::RefreshOptionMaterial()
{
	if (nullptr == m_optionMaterial) return;
	for (enumType i = 0; i < m_items.size(); ++i)
	{
		auto material = m_optionMaterial(i);
		CacheString(toX(m_position.x, i, m_xOffset, m_itemPerLine), toY(m_position.y + 1, i, m_yOffset, m_itemPerLine), material.first, material.second);
	}
}

bool PainterToogleGroup::UpdateOption(MOUSE_EVENT_RECORD mer)
{
	int mx = mer.dwMousePosition.X / 2 - m_position.x;
	int my = mer.dwMousePosition.Y - m_position.y;
	if (mx <= 0 || mx >= MSG_WIDTH || my < 0 && my >= m_lineCount * m_yOffset) return false;

	enumType option = mx / m_xOffset + (my / m_yOffset) * m_itemPerLine;
	if (option >= m_items.size()) return false;

	m_optionChangedHandler(option);
	if (nullptr != m_option)
		*m_option = option;

	return true;
}

int PainterToogleGroup::ToNewGroupY(const PainterToogleGroup & upperGroup)
{
	return YOffset(upperGroup.m_position.y, upperGroup.m_yOffset, upperGroup.m_lineCount);
}

EditorPainter::EditorPainter() : game::Renderer(MSG_WIDTH, MSG_HEIGHT, game::RenderType::UICanvas), m_position(GAME_WIDTH, 0),
	m_type(E_EditType::PenEraser), m_colorType(E_ColorType::SetForeColor), m_cellType(E_StaticCellType::OpenSpace), m_pointSet({ 0,0 }, false)
{
	m_model = __LevelModel::NewLevel();
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

void EditorPainter::set_ColorType(E_ColorType colorType)
{
	m_colorType = colorType;
}

void EditorPainter::set_ForeColor(E_4BitColor foreColor)
{
	m_cellColors[m_cellType].fore = foreColor;
	if (m_pointSet.isValid)
		m_model->SetType(m_pointSet.position, m_cellType, m_cellColors[m_cellType]);
}

void EditorPainter::set_BackColor(E_4BitColor backColor)
{
	m_cellColors[m_cellType].back = backColor;
	if (m_pointSet.isValid)
		m_model->SetType(m_pointSet.position, m_cellType, m_cellColors[m_cellType]);
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
				m_model->AppendPlayer(position, m_cellColors[m_cellType]);
			else if (m_type == E_EditType::HollowSet)
				m_model->SetHollowLand(minPos, maxPos, m_cellType, m_cellColors[m_cellType]);
			else if (m_type == E_EditType::CloseySet)
				m_model->SetCloseyLand(minPos, maxPos, m_cellType, m_cellColors[m_cellType]);
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
	m_model->SetType(position, m_cellType, m_cellColors[m_cellType]);
	return true;
}

bool EditorPainter::DrawEditRightKey(Vector2 &position)
{
	if (!m_pointSet.Clear(m_model))
		m_model->SetType(position, E_StaticCellType::OpenSpace, DEFAULT_COLOR);
	if (m_cellType == E_StaticCellType::GermPoint)
		m_model->RemovePlayer(position);
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
	else if (mx > 0 && mx < MSG_WIDTH && my == LevelY && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
	{
		int operation = (mx - 1) / LevelXOffset;
		switch (operation)
		{
		case 0: m_model->ToLast(); break;
		case 1: 
			if (m_model->ExistNext())
				m_model->ToNext();
			else
			{
				m_model->CreateNext();
				m_model->ToNext();
				m_model->SetHollowLand({ 0, 0 }, { (GAME_WIDTH + 0 - 1), (GAME_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand, DEFAULT_COLOR);
			}
			break;
		case 2:
			m_model->DeleteNext();
			break;
		default:
			break;
		}
	}
	else if (my == OptionY && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
	{
		if (mx >= OptionXOffset * 0 + 2 && mx <= OptionXOffset * 1)
			New();
		else if (mx >= OptionXOffset * 1 + 2 && mx <= OptionXOffset * 2)
			Load();
		else if (mx >= OptionXOffset * 2 + 2 && mx <= OptionXOffset * 3)
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
	m_model->Clear();
	m_model->SetHollowLand({ 0, 0 }, { (GAME_WIDTH + 0 - 1), (GAME_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand, DEFAULT_COLOR);
}

void EditorPainter::Load()
{
	m_model->LoadByBrowse();
}

void EditorPainter::Save()
{
	m_model->SaveByBrowse();
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
	for (int i = 0; i < LevelCount; ++i)
	{
		auto color = (i == 0 && !m_model->ExistLast()) ? ConsoleColor(E_4BitColor::Gray, DEFAULT_BACK_COLOR) : DEFAULT_COLOR;
		auto startIndex = BrushCount + BModeCount + ColorCount + CModeCount + OptionCount;
		switch (i)
		{
		case 1: startIndex += m_model->ExistNext() ? 2 : 1; break;
		case 2: startIndex += 3; break;
		case 0: default: break;
		}
		CacheString(toX(LevelX, i, LevelXOffset, LevelCountPerLine), toY(LevelY, i, LevelYOffset, LevelCountPerLine), itemNames[startIndex], color);
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
	map.RenderModel(m_model);
	return continueFlag;
}

bool PointSet::Clear(LevelModel & model)
{
	if (!isValid) return false;

	isValid = false;
	model->SetType(position, E_StaticCellType::OpenSpace, DEFAULT_COLOR);
	return true;

}
