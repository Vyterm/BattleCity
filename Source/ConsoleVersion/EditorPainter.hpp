#ifndef EDITOR_PAINTER_HPP_INCLUDED
#define EDITOR_PAINTER_HPP_INCLUDED

#include "GameModel.hpp"
#include "GameMap.hpp"
#include "GameRender.hpp"
#include "winapi.hpp"
#include <string>
#include <vector>
#include <functional>

enum class E_EditType : enumType
{
	PenEraser,
	HollowSet,
	CloseySet,
};

enum class E_ColorType : enumType
{
	SetForeColor,
	SetBackColor,
};

enum class E_EditMode : enumType
{
	LeftKey,
	RightKey,
};

struct PointSet
{
	PointSet(Vector2 position, bool isValid) : position(position), isValid(isValid) {}
	Vector2 position;
	bool isValid;
	bool Clear(LevelModel &model);
};
class PainterToogleGroup : game::Renderer
{
public:
	using EventHandler = std::function<void(enumType)>;
	using OptionMaterial = std::function<std::pair<std::string, ConsoleColor>(enumType)>;
private:
	std::vector<std::string> m_items;
	EventHandler m_optionChangedHandler;
	enumType *m_option;
	OptionMaterial m_optionMaterial;
	Vector2 m_position;
	Vector2 m_drawPosition;
	int m_xOffset;
	int m_yOffset;
	int m_itemPerLine;
	int m_lineCount;
public:
	const Vector2& getPosition() const { return m_drawPosition; }
private:
	void RenderOption();
public:
	PainterToogleGroup(std::vector<std::string> &&itemHints, EventHandler optionChangedHandler,
		int x, int y, int xOffset, enumType *option = nullptr, OptionMaterial optionMaterial = nullptr);
	void RefreshOption(enumType *option);
	void RefreshOptionMaterial();
	bool UpdateOption(MOUSE_EVENT_RECORD mer);

	static int ToNewGroupY(const PainterToogleGroup &upperGroup);
};

class EditorPainter : game::Renderer
{
	Vector2 m_position;
	LevelModel m_model;
	GameMap map;
	E_EditType m_type;
	E_ColorType m_colorType;
	std::map<E_StaticCellType, ConsoleColor> m_cellColors;
	E_StaticCellType m_cellType;
	PointSet m_pointSet;
public:
	E_EditType get_Type() const { return m_type; }
	void set_Type(E_EditType type);
	E_ColorType get_ColorType() const { return m_colorType; }
	void set_ColorType(E_ColorType colorType);
	E_4BitColor get_ForeColor() { return m_cellColors[m_cellType].fore; }
	void set_ForeColor(E_4BitColor foreColor);
	E_4BitColor get_BackColor() { return m_cellColors[m_cellType].back; }
	void set_BackColor(E_4BitColor backColor);
	E_StaticCellType get_CellType() const { return m_cellType; }
	void set_CellType(E_StaticCellType cellType);
	const Vector2& getPosition() const { return m_position; }
private:
	bool DrawEditLeftKey(Vector2 &position);
	bool DrawEditRightKey(Vector2 &position);

	bool KeyEventProc(KEY_EVENT_RECORD ker);
	bool MouseEventProc(MOUSE_EVENT_RECORD mer);

	void TryUpdatePainter(const MOUSE_EVENT_RECORD &mer);
	void TryPaint(const MOUSE_EVENT_RECORD &mer);
public:
	EditorPainter();
	bool IsDoublePoint() const;
	bool DrawEdit(Vector2 position, E_EditMode mode);

	void Rerender();
	bool Update(const INPUT_RECORD &record);

	void New();
	void Load();
	void Save();
};

#endif