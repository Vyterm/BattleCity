#include "GameModel.hpp"

#pragma region Tank Model

static const int MIN_HEALTH = 1;
static const int MAX_HEALTH = 6;

std::map<E_TankType, TankModel> TankModel::StandardTankModels = {
	{E_TankType::Light,		TankModel({ 0, 0 }, E_TankType::Light,	1,	4,	6,	1,	SPEED_LIGHT_TANK)},
	{E_TankType::Medium,	TankModel({ 0, 0 }, E_TankType::Medium,	1,	6,	6,	2,	SPEED_MEDIUM_TANK)},
	{E_TankType::Heavy,		TankModel({ 0, 0 }, E_TankType::Heavy,	1,	6,	6,	3,	SPEED_HEAVY_TANK)},
	{E_TankType::Assault,	TankModel({ 0, 0 }, E_TankType::Assault,	1,	6,	4,	2,	SPEED_ASSAULT_TANK)},
};

TankModel::TankModel(const Vector2 &germPosition, E_TankType type)
	: germPosition(germPosition), type(type), maxLife(1), maxHealth(StandardTankModels[type].maxHealth), attack(StandardTankModels[type].maxHealth),
	defense(StandardTankModels[type].defense), speed(StandardTankModels[type].speed), color(DEFAULT_FORE_COLOR)
{
}

TankModel::TankModel(const Vector2 &germPosition, E_TankType type, int maxHealth, int attack, int defense, int speed)
	: germPosition(germPosition), type(type), maxLife(1), maxHealth(maxHealth), attack(attack), defense(defense), speed(speed), color(DEFAULT_FORE_COLOR)
{
	this->maxHealth = this->maxHealth < MIN_HEALTH ? MIN_HEALTH : this->maxHealth > MAX_HEALTH ? MAX_HEALTH : this->maxHealth;
}

TankModel::TankModel(const Vector2 &germPosition, E_TankType type, int maxLife, E_4BitColor color)
	: germPosition(germPosition), type(type), maxLife(maxLife), maxHealth(StandardTankModels[type].maxHealth),
	attack(StandardTankModels[type].attack), defense(StandardTankModels[type].defense), speed(StandardTankModels[type].speed), color(color)
{
	if (this->maxLife < 1)
		this->maxLife = 1;
}

TankModel::TankModel(const Vector2 &germPosition, E_TankType type, int maxLife, int maxHealth, int attack, int defense, int speed, E_4BitColor color)
	: germPosition(germPosition), type(type), maxLife(maxLife), maxHealth(maxHealth), attack(attack), defense(defense), speed(speed), color(color)
{
	if (this->maxLife <= 0)
		this->maxLife = 1;
	this->maxHealth = this->maxHealth < MIN_HEALTH ? MIN_HEALTH : this->maxHealth > MAX_HEALTH ? MAX_HEALTH : this->maxHealth;
}

Vector2 TankModel::ToEnemyPosition(E_EnemyPosition eep)
{
	return { 1 + int(eep) * (GAME_WIDTH / 2 - 2), 1 };
}

#pragma endregion


LevelModel::LevelModel() { }

LevelModel::~LevelModel() { }

void LevelModel::Clear()
{
	for (int x = 0; x < WIDTH; ++x)
		for (int y = 0; y < HEIGHT; ++y)
		{
			m_cellModels[x][y] = E_StaticCellType::OpenSpace;
			m_cellModels[x][y].color = DEFAULT_COLOR;
		}
	m_playerModels.erase(m_playerModels.begin(), m_playerModels.end());
}

#pragma region Land Shape

void LevelModel::SetHollowLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType, ConsoleColor color)
{
	for (int y = startPos.y; y <= endPos.y; ++y)
		for (int x = startPos.x; x <= endPos.x; ++x)
			if (x == startPos.x || x == endPos.x || y == startPos.y || y == endPos.y)
				m_cellModels[x][y] = { staticType, color };
			else
				m_cellModels[x][y] = E_StaticCellType::OpenSpace;
}

void LevelModel::SetCloseyLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType, ConsoleColor color)
{
	for (int y = startPos.y; y <= endPos.y; ++y)
		for (int x = startPos.x; x <= endPos.x; ++x)
			m_cellModels[x][y] = { staticType, color };
}

void LevelModel::SetType(Vector2 position, E_StaticCellType type, ConsoleColor color)
{
	if (type == E_StaticCellType::GermPoint)
		AppendPlayer(position, color);
	else
		Index(position.x, position.y) = { type, color };
}

E_StaticCellType LevelModel::GetType(const Vector2 & position) const
{
	return Index(position.x, position.y).type;
}

ConsoleColor LevelModel::GetColor(const Vector2 & position) const
{
	return Index(position).color;
}

#pragma endregion

#pragma region Germ Vector2

void LevelModel::AppendPlayer(Vector2 position, ConsoleColor color)
{
	RemovePlayer(position);
	m_playerModels.push_back({ position, E_TankType::Assault, 3, color.fore });
	if (m_playerModels.size() > MAX_PLAYER_COUNT)
		m_playerModels.pop_front();
}

void LevelModel::RemovePlayer(Vector2 position)
{
	Vector2 playerRectLT = { 0, 0 };
	Vector2 playerRectRB = { 3, 3 };
	for (auto iter = m_playerModels.begin(); iter != m_playerModels.end();)
	{
		auto distance = iter->germPosition - position;
		distance.x *= distance.x < 0 ? -1 : 1;
		distance.y *= distance.y < 0 ? -1 : 1;
		if (distance >= playerRectLT && distance < playerRectRB)
			iter = m_playerModels.erase(iter);
		else
			++iter;
	}
}

const std::deque<TankModel>& LevelModel::PlayerModels() const
{
	return m_playerModels;
}

void LevelModel::AppendEnemy(E_TankType enemyType)
{
	m_enemyModels.push_back({ TankModel::ToEnemyPosition(TankModel::E_EnemyPosition(m_enemyModels.size() % TankModel::EP_Count)), enemyType });
}

void LevelModel::RemoveEnemy()
{
	m_enemyModels.pop_back();
}

const std::deque<TankModel>& LevelModel::EnemyModels() const
{
	return m_enemyModels;
}

#pragma endregion

#pragma region Save & Load

std::ostream & operator<<(std::ostream & os, const LandModel & model)
{
	os << int(model.type) << " " << model.color;
	return os;
}

std::istream & operator>>(std::istream & is, LandModel & model)
{
	int it;
	is >> it >> model.color;
	model.type = E_StaticCellType(it);
	return is;
}

std::ostream & operator<<(std::ostream & os, const TankModel & model)
{
	os << model.germPosition << enumType(model.type) << " " << enumType(model.color) << " "
		<< model.maxLife << " " << model.maxHealth << " " << model.attack << " " << model.defense << " " << model.speed << " ";
	return os;
}

std::istream & operator>>(std::istream & is, TankModel & model)
{
	enumType type, color;
	is >> model.germPosition >> type >> color >> model.maxLife >> model.maxHealth
		>> model.attack >> model.defense >> model.speed;
	model.type = E_TankType(type);
	model.color = E_4BitColor(color);
	return is;
}

std::ostream & operator<<(std::ostream & os, const PlayerModel & model)
{
	os << model.position << " ";
	os << int(model.foreColor) << " ";
	return os;
}

std::istream & operator>>(std::istream & is, PlayerModel & model)
{
	int foreColor;
	is >> model.position;
	is >> foreColor;
	model.foreColor = E_4BitColor(foreColor);
	return is;
}

std::ostream & operator<<(std::ostream & os, LevelModel & mapModel)
{
	os << VERSION[0] << " " << VERSION[1] << " " << VERSION[2] << " ";
	for (int x = 0; x < WIDTH; ++x)
		for (int y = 0; y < HEIGHT; ++y)
			os << mapModel.m_cellModels[x][y];
	os << mapModel.m_playerModels.size() << " ";
	for (auto &pm : mapModel.m_playerModels) os << pm;
	os << mapModel.m_enemyModels.size() << " ";
	for (auto &et : mapModel.m_enemyModels) os << et;
	return os;
}

std::istream & operator>>(std::istream & is, LevelModel & mapModel)
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
		if (version[1] < 3 || version[2] < 11)
		{
			Vector2 pm;
			is >> pm;
			mapModel.m_playerModels.push_back({ pm, E_TankType::Assault, 3, i == 0 ? E_4BitColor::LCyan : E_4BitColor::LWhite });
		}
		else
		{
			TankModel tank;
			is >> tank;
			mapModel.m_playerModels.push_back(tank);
		}
	}
	is >> size;
	if (version[1] < 3 || version[2] < 11)
	{
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Medium);
		mapModel.AppendEnemy(E_TankType::Medium);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Medium);
		mapModel.AppendEnemy(E_TankType::Medium);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Light);
		mapModel.AppendEnemy(E_TankType::Heavy);
		mapModel.AppendEnemy(E_TankType::Heavy);
	}
	else
	{
		for (size_t i = 0; i < size; ++i)
		{
			TankModel tank;
			is >> tank;
			mapModel.m_enemyModels.push_back(tank);
		}
	}
	return is;
}

#pragma endregion
