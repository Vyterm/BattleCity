#ifndef GAME_EDITOR_HPP
#define GAME_EDITOR_HPP

#include "GameDefines.hpp"

#include <map>
#include <set>
#include <vector>
#include <deque>
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

struct LandModel
{
	E_StaticCellType type;
	ConsoleColor color;
	LandModel() : type(E_StaticCellType::OpenSpace), color(DEFAULT_COLOR) { }
	LandModel(E_StaticCellType type, ConsoleColor color) : type(type), color(color) { }
	LandModel& operator=(E_StaticCellType type) { this->type = type; return *this; }
	LandModel& operator=(ConsoleColor foreColor) { this->color = color; return *this; }
	friend std::ostream& operator<<(std::ostream& os, const LandModel& model);
	friend std::istream& operator>>(std::istream& is, LandModel& model);
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
	friend std::ostream& operator<<(std::ostream& os, const TankModel& model);
	friend std::istream& operator>>(std::istream& is, TankModel& model);
};

struct PlayerModel
{
	Vector2 position;
	E_4BitColor foreColor;
	friend std::ostream& operator<<(std::ostream& os, const PlayerModel& model);
	friend std::istream& operator>>(std::istream& is, PlayerModel& model);
};

constexpr int WIDTH = GAME_WIDTH;
constexpr int HEIGHT = GAME_HEIGHT;
class LevelModel
{
public:
private:
	LandModel m_cellModels[WIDTH][HEIGHT];
	std::deque<Vector2> m_germPoints;
	std::deque<TankModel> m_enemyTanks;
	LandModel& Index(int x, int y) { return m_cellModels[x][y]; }
	const LandModel& Index(int x, int y) const { return m_cellModels[x][y]; }
	LandModel& Index(const Vector2 &position) { return m_cellModels[position.x][position.y]; }
	const LandModel& Index(const Vector2 &position) const { return m_cellModels[position.x][position.y]; }
public:
	LevelModel();
	~LevelModel();
	void Clear();

	void SetHollowLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType, ConsoleColor color);
	void SetCloseyLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType, ConsoleColor color);
	void SetType(Vector2 position, E_StaticCellType type, ConsoleColor color);
	E_StaticCellType GetType(const Vector2 &position) const;
	ConsoleColor GetColor(const Vector2 &position) const;

	void SetPlayer(Vector2 position, ConsoleColor color);
	std::set<Vector2> GermPoints() const;

	friend std::ostream& operator<<(std::ostream& os, LevelModel &mapModel);
	friend std::istream& operator>>(std::istream& is, LevelModel &mapModel);
};

#endif