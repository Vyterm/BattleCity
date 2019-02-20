#include "GameModel.hpp"

#pragma region Tank Model

static const int MIN_HEALTH = 1;
static const int MAX_HEALTH = 6;

std::map<E_TankType, TankModel> TankModel::StandardTankModels = {
	{E_TankType::Light,		TankModel(E_TankType::Light,	1,	4,	6,	1,	80)},
	{E_TankType::Medium,	TankModel(E_TankType::Medium,	1,	6,	6,	2,	0)},
	{E_TankType::Heavy,		TankModel(E_TankType::Heavy,	1,	6,	6,	3,	-80)},
	{E_TankType::Assault,	TankModel(E_TankType::Assault,	1,	6,	4,	2,	30)},
};

TankModel::TankModel(E_TankType type)
	: type(type), maxLife(1), maxHealth(StandardTankModels[type].maxHealth), attack(StandardTankModels[type].maxHealth),
	defense(StandardTankModels[type].defense), speed(StandardTankModels[type].speed), color(DEFAULT_FORE_COLOR)
{
}

TankModel::TankModel(E_TankType type, int maxHealth, int attack, int defense, int speed)
	: type(type), maxLife(1), maxHealth(maxHealth), attack(attack), defense(defense), speed(speed), color(DEFAULT_FORE_COLOR)
{
	this->maxHealth = this->maxHealth < MIN_HEALTH ? MIN_HEALTH : this->maxHealth > MAX_HEALTH ? MAX_HEALTH : this->maxHealth;
}

TankModel::TankModel(E_TankType type, int maxLife, E_4BitColor color)
	: type(type), maxLife(maxLife), maxHealth(StandardTankModels[type].maxHealth),
	attack(StandardTankModels[type].attack), defense(StandardTankModels[type].defense), speed(StandardTankModels[type].speed), color(color)
{
	if (this->maxLife < 1)
		this->maxLife = 1;
}

TankModel::TankModel(E_TankType type, int maxLife, int maxHealth, int attack, int defense, int speed, E_4BitColor color)
	: type(type), maxLife(maxLife), maxHealth(maxHealth), attack(attack), defense(defense), speed(speed), color(color)
{
	if (this->maxLife <= 0)
		this->maxLife = 1;
	this->maxHealth = this->maxHealth < MIN_HEALTH ? MIN_HEALTH : this->maxHealth > MAX_HEALTH ? MAX_HEALTH : this->maxHealth;
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
	m_germPoints.erase(m_germPoints.begin(), m_germPoints.end());
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
		SetPlayer(position, color);
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

void LevelModel::SetPlayer(Vector2 position, ConsoleColor color)
{
	m_germPoints.push_back(position);
	if (m_germPoints.size() > MAX_PLAYER_COUNT)
		m_germPoints.pop_front();
}

std::set<Vector2> LevelModel::GermPoints() const
{
	return std::set<Vector2>(m_germPoints.begin(), m_germPoints.end());
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
	os << enumType(model.type) << " "
		<< enumType(model.color) << " "
		<< model.maxLife << " "
		<< model.maxHealth << " "
		<< model.attack << " "
		<< model.defense << " "
		<< model.speed << " ";
	return os;
}

std::istream & operator>>(std::istream & is, TankModel & model)
{
	enumType type, color;
	is >> type >> color >> model.maxLife >> model.maxHealth
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
	os << mapModel.m_germPoints.size() << " ";
	for (auto &gp : mapModel.m_germPoints) os << gp;
	os << mapModel.m_enemyTanks.size() << " ";
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
		Vector2 pm;
		is >> pm;
		mapModel.m_germPoints.push_back(pm);
	}
	is >> size;
	return is;
}

#pragma endregion
