#ifndef GAME_EDITOR_HPP
#define GAME_EDITOR_HPP

#include "GameDefines.hpp"

#include <map>
#include <set>
#include <vector>
#include <deque>
#include <fstream>
#include <memory>

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
	EnemyGerm,
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

constexpr auto SPEED_BULLET = 80;
constexpr auto SPEED_LIGHT_TANK = 50;
constexpr auto SPEED_MEDIUM_TANK = 20;
constexpr auto SPEED_HEAVY_TANK = 0;
constexpr auto SPEED_ASSAULT_TANK = 30;

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
	Vector2 germPosition;
	E_TankType type;
	E_4BitColor color;
	int maxLife;
	int maxHealth;
	int attack;
	int defense;
	int speed;
	TankModel() = default;
	TankModel(const Vector2 &germPosition, E_TankType type);
	TankModel(const Vector2 &germPosition, E_TankType type, int maxHealth, int attack, int defense, int speed);
	TankModel(const Vector2 &germPosition, E_TankType type, int maxLife, E_4BitColor color = DEFAULT_FORE_COLOR);
	TankModel(const Vector2 &germPosition, E_TankType type, int maxHealth, int attack, int defense, int speed, int maxLife, E_4BitColor color = DEFAULT_FORE_COLOR);
	friend std::ostream& operator<<(std::ostream& os, const TankModel& model);
	friend std::istream& operator>>(std::istream& is, TankModel& model);
	enum E_EnemyPosition { EP_LeftUp = 0, EP_CenterUp = 1, EP_RightUp = 2, EP_Count = 3 };
	static Vector2 ToEnemyPosition(E_EnemyPosition eep);
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
class __LevelModel
{
public:
private:
	LandModel m_cellModels[WIDTH][HEIGHT];
	std::deque<TankModel> m_playerModels;
	std::deque<TankModel> m_enemyModels;
	size_t m_stage;
	__LevelModel *m_last = nullptr;
	__LevelModel *m_next = nullptr;
	LandModel& Index(int x, int y) { return m_cellModels[x][y]; }
	const LandModel& Index(int x, int y) const { return m_cellModels[x][y]; }
	LandModel& Index(const Vector2 &position) { return m_cellModels[position.x][position.y]; }
	const LandModel& Index(const Vector2 &position) const { return m_cellModels[position.x][position.y]; }
private:
	__LevelModel(size_t stage);
	__LevelModel(const __LevelModel &lm) = delete;
	__LevelModel& operator=(const __LevelModel &lm) = delete;
	__LevelModel(__LevelModel &&lm) = default;
	__LevelModel& operator=(__LevelModel &&lm) = default;
	static __LevelModel CacheForSwap;
public:
	static std::shared_ptr<__LevelModel> NewLevel();
	~__LevelModel();
	bool LoadByBrowse();
	bool SaveByBrowse();
	void Clear();

	bool ExistLast() const;
	bool ExistNext() const;
	bool CreateLast();
	bool CreateNext();
	bool DeleteLast();
	bool DeleteNext();
	bool ToLast();
	bool ToNext();

	void SetHollowLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType, ConsoleColor color);
	void SetCloseyLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType, ConsoleColor color);
	void SetType(Vector2 position, E_StaticCellType type, ConsoleColor color);
	size_t GetStage() const;
	E_StaticCellType GetType(const Vector2 &position) const;
	ConsoleColor GetColor(const Vector2 &position) const;

	void AppendPlayer(Vector2 position, ConsoleColor color);
	void RemovePlayer(Vector2 position);
	const std::deque<TankModel>& PlayerModels() const;
	void AppendEnemy(E_TankType enemyType);
	void RemoveEnemy();
	const std::deque<TankModel>& EnemyModels() const;

	friend std::ostream& operator<<(std::ostream& os, const __LevelModel &mapModel);
	friend std::istream& operator>>(std::istream& is, __LevelModel &mapModel);
};

using LevelModel = std::shared_ptr<__LevelModel>;

#endif