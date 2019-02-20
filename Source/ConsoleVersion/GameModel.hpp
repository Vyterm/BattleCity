#ifndef GAME_EDITOR_HPP
#define GAME_EDITOR_HPP

#include "GameDefines.hpp"

#include <map>
#include <vector>
#include <fstream>

#pragma region Cell Defines

enum class E_StaticCellType
{
	OpenSpace,
	JebelLand,
	GrassLand,
	MagmaLand,
	FrostLand,
	EarthWall,

	GermPoint,
};
constexpr const char* StaticCellImages[] = { "  ", "■", "≡", "≈", "～", "▓", "●", "※" };

#pragma endregion

struct CellModel
{
	E_StaticCellType type;
	ConsoleColor color;
	CellModel() : type(E_StaticCellType::OpenSpace), color(DEFAULT_COLOR) { }
	CellModel(E_StaticCellType type, ConsoleColor color) : type(type), color(color) { }
	CellModel& operator=(E_StaticCellType type) { this->type = type; return *this; }
	CellModel& operator=(ConsoleColor foreColor) { this->color = color; return *this; }
	friend std::ostream& operator<<(std::ostream& os, CellModel& model)
	{
		os << int(model.type) << " " << model.color;
		return os;
	}
	friend std::istream& operator>>(std::istream& is, CellModel& model)
	{
		int it;
		is >> it >> model.color;
		model.type = E_StaticCellType(it);
		return is;
	}
};

enum class E_TankType
{
	// 轻型坦克
	Light,
	// 中型坦克
	Medium,
	// 突击坦克
	Assault,
	// 重型坦克
	Heavy,
};

struct TankModel
{
	static std::map<E_TankType, TankModel> StandardTankModels;
	E_TankType type;
	E_4BitColor color;
	int maxLife;
	int maxHealth;
	int attack;
	int defense;
	int speed;
	TankModel() = default;
	TankModel(E_TankType type);
	TankModel(E_TankType type, int maxHealth, int attack, int defense, int speed);
	TankModel(E_TankType type, int maxLife, E_4BitColor color = DEFAULT_FORE_COLOR);
	TankModel(E_TankType type, int maxHealth, int attack, int defense, int speed, int maxLife, E_4BitColor color = DEFAULT_FORE_COLOR);
};

struct PlayerModel
{
	Vector2 position;
	E_4BitColor foreColor;
	friend std::ostream& operator<<(std::ostream& os, PlayerModel& model)
	{
		os << model.position << " ";
		os << int(model.foreColor) << " ";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, PlayerModel& model)
	{
		int foreColor;
		is >> model.position;
		is >> foreColor;
		model.foreColor = E_4BitColor(foreColor);
		return is;
	}
};

class LevelModel
{
public:
	static const int WIDTH = GAME_WIDTH;
	static const int HEIGHT = GAME_HEIGHT;
private:
	CellModel m_cellModels[WIDTH][HEIGHT];
	std::vector<Vector2> m_germPoints;
	//const CellModel& get_Index(size_t x, size_t y) const { return m_cellModels[x + y * m_width]; }
	//void set_Index(size_t x, size_t y, const CellModel &cell) { m_cellModels[x + y * m_width] = cell; }
	CellModel& Index(int x, int y) { return m_cellModels[x][y]; }
	const CellModel& Index(int x, int y) const { return m_cellModels[x][y]; }
	CellModel& Index(const Vector2 &position) { return m_cellModels[position.x][position.y]; }
	const CellModel& Index(const Vector2 &position) const { return m_cellModels[position.x][position.y]; }
public:
	LevelModel()
	{
	}
	~LevelModel() { }

	void Clear()
	{
		for (int x = 0; x < WIDTH; ++x)
			for (int y = 0; y < HEIGHT; ++y)
			{
				m_cellModels[x][y] = E_StaticCellType::OpenSpace;
				m_cellModels[x][y].color = DEFAULT_COLOR;
			}
		m_germPoints.erase(m_germPoints.begin(), m_germPoints.end());
	}

	#pragma region Land Shape

	void SetHollowLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType, ConsoleColor color)
	{
		for (int y = startPos.y; y <= endPos.y; ++y)
			for (int x = startPos.x; x <= endPos.x; ++x)
				if (x == startPos.x || x == endPos.x || y == startPos.y || y == endPos.y)
					m_cellModels[x][y] = { staticType, color };
				else
					m_cellModels[x][y] = E_StaticCellType::OpenSpace;
	}

	void SetCloseyLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType, ConsoleColor color)
	{
		for (int y = startPos.y; y <= endPos.y; ++y)
			for (int x = startPos.x; x <= endPos.x; ++x)
				m_cellModels[x][y] = { staticType, color };
	}

	void SetCross(Vector2 position)
	{
		Index(position.x - 1, position.y) = Index(position.x, position.y) = Index(position.x + 1, position.y)
			= Index(position.x, position.y - 1) = Index(position.x, position.y + 1) = E_StaticCellType::JebelLand;
	}

	void SetType(Vector2 position, E_StaticCellType type, ConsoleColor color)
	{
		if (type == E_StaticCellType::GermPoint)
			SetPlayer(position, color);
		else
			Index(position.x, position.y) = { type, color };
	}

	E_StaticCellType GetType(const Vector2 &position) const
	{
		return Index(position.x, position.y).type;
	}

	ConsoleColor GetColor(const Vector2 &position) const
	{
		return Index(position).color;
	}

	#pragma endregion

	#pragma region Germ Vector2

	void SetPlayer(Vector2 position, ConsoleColor color)
	{
		Index(position) = { E_StaticCellType::GermPoint, color };
		m_germPoints.push_back(position);
		if (PlayerCount() > MAX_PLAYER_COUNT)
		{
			auto invalidPlayer = m_germPoints[0];
			Index(invalidPlayer) = { E_StaticCellType::OpenSpace, DEFAULT_COLOR };
			m_germPoints.erase(m_germPoints.begin());
		}
	}
	const Vector2& GetPlayer(size_t index) const { return m_germPoints[index]; }
	size_t PlayerCount() const { return m_germPoints.size(); }

	#pragma endregion

	#pragma region Save & Load

	friend std::ostream& operator<<(std::ostream& os, LevelModel &mapModel)
	{
		os << VERSION[0] << " " << VERSION[1] << " " << VERSION[2] << " ";
		for (int x = 0; x < WIDTH; ++x)
			for (int y = 0; y < HEIGHT; ++y)
				os << mapModel.m_cellModels[x][y];
		os << mapModel.m_germPoints.size() << " ";
		for (auto &gp : mapModel.m_germPoints) os << gp;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, LevelModel &mapModel)
	{
		int version[3];
		is >> version[0] >> version[1] >> version[2];
		for (int x = 0; x < WIDTH; ++x)
			for (int y = 0; y < HEIGHT; ++y)
				is >> mapModel.m_cellModels[x][y];
		size_t size;
		is >> size;
		for (size_t i = 0; i < size; ++i)
		{
			Vector2 pm;
			is >> pm;
			mapModel.m_germPoints.push_back(pm);
		}
		return is;
	}

	#pragma endregion
};

#endif