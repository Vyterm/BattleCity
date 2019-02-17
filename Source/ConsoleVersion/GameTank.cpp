#include "GameTank.hpp"
#include "ColliderDefines.hpp"

constexpr size_t TANK_WIDTH = 3;
constexpr size_t TANK_HEIGHT = 3;
constexpr int MIN_HEALTH = 1;
constexpr int MAX_HEALTH = 6;

static const string TankParts[4][3][4] =
{
  {
	{"®ç©ß®é", "®ç©•®é", "®ç©◊®é", "®ç©◊®é"},
	{"©«°Ò©œ", "©«°Ò©œ", "©•°Ò©ß", "©ß°Ò©•"},
	{"®ê©•®è", "®ê©ß®è", "®ê©ﬂ®è", "®ê©ﬂ®è"}
  },
  {
	{"©≥©ß©∑", "©≥©◊©∑", "©≥©◊©∑", "©≥©◊©∑"},
	{"©«°Ò©œ", "©«°Ò©œ", "©•°Ò©œ", "©«°Ò©•"},
	{"©ª©ﬂ©ø", "©ª©ß©ø", "©ª©ﬂ©ø", "©ª©ﬂ©ø"}
  },
  {
	{"©≥©ß©∑", "®ç©•®é", "©≥©◊®é", "®ç©◊©∑"},
	{"©«°Ò©œ", "©«°Ò©œ", "©•°Ò©ß", "©ß°Ò©•"},
	{"®ê©•®è", "©ª©ß©ø", "©ª©ﬂ®è", "®ê©ﬂ©ø"}
  },
  {
	{"®X©ß®[", "®X®j®[", "®X®j®[", "®X®j®["},
	{"®d®Ä®g", "®d®Ä®g", "©•®Ä®g", "®d®Ä©•"},
	{"®^®m®a", "®^©ß®a", "®^®m®a", "®^®m®a"}
  }
};

static const E_4BitColor HealthColors[] =
{
	E_4BitColor::Red,
	E_4BitColor::LRed,
	E_4BitColor::Yellow,
	E_4BitColor::LYellow,
	E_4BitColor::Green,
	E_4BitColor::LGreen
};

std::map<E_TankType, TankModel> TankModel::StandardTankModels =  {
	{E_TankType::Light,		TankModel(E_TankType::Light,	1,	4,	6,	1,	80	)},
	{E_TankType::Medium,	TankModel(E_TankType::Medium,	1,	6,	6,	2,	0	)},
	{E_TankType::Heavy,		TankModel(E_TankType::Heavy,	1,	6,	6,	3,	-80	)},
	{E_TankType::Assault,	TankModel(E_TankType::Assault,	1,	6,	4,	2,	30	)},
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

inline const string& GetStringByState(size_t y, Direction2D direction, size_t modelType = 0)
{
	auto directionIndex = 0;
	switch (direction)
	{
	case Direction2D::Left: directionIndex = 2; break;
	case Direction2D::Right: directionIndex = 3; break;
	case Direction2D::Up: directionIndex = 0; break;
	case Direction2D::Down: directionIndex = 1; break;
	case Direction2D::None:
	default: break;
	}
	return TankParts[modelType][y][directionIndex];
}

bool Tank::isEnemy() const
{
	return m_isEnemy;
}

bool Tank::isAlive() const
{
	return m_lifePoint > 0;
}

Tank::Tank(TankModel model, bool isEnemy) :
	game::Renderer(TANK_WIDTH, TANK_HEIGHT, game::RenderType::ActiveLayer0),
	game::Collider(false), m_type(model.type), m_color(model.color), m_isEnemy(isEnemy),
	m_maxLife(model.maxLife), m_lifePoint(model.maxLife), m_maxHealth(model.maxHealth), m_healthPoint(model.maxHealth),
	m_attack(model.attack), m_defense(model.defense), m_direction(Direction2D::Up)
{
	SetDrawActive(false);
}

Tank::~Tank()
{
}

void Tank::Reset()
{
	Regerm();
	m_lifePoint = m_maxLife;
	m_healthPoint = m_maxHealth;
	SetDrawActive(true);
	setColliderActive(true);
	DrawTank();
}

void Tank::Clear()
{
	ClearCache();
	setColliderActive(false);
}

void Tank::ReduceHealth(int attack)
{
	auto damage = attack - m_defense;
	m_healthPoint -= damage > 0 ? damage : 0;
	if (m_healthPoint > 0) return;
	--m_lifePoint;
	m_healthPoint = m_maxHealth;
	if (isAlive())
		Regerm();
	else
		Clear();
}

void Tank::Move(Vector2 target)
{
	Vector2 tmp = m_position;
	m_position = target;
	m_moveable = true;
	SimulateMove();
	if (!m_moveable)
		m_position = tmp;
}

void Tank::OnCollision(Collider & collider)
{
	if (collider.getType() == COLLIDER_TYPE_JEBEL_LANDSPACE ||
		collider.getType() == COLLIDER_TYPE_EARTH_LANDSPACE ||
		collider.getType() == COLLIDER_TYPE_ENEMY_TANK ||
		collider.getType() == COLLIDER_TYPE_FRIEND_TANK ||
		collider.getType() == COLLIDER_TYPE_PLAYER_BASE)
		m_moveable = false;
}

bool Tank::Contains(const Vector2 & position)
{
	return position.x - m_position.x < TANK_WIDTH && position.y - m_position.y < TANK_HEIGHT;
}

bool Tank::SetPositionByIndex(size_t index, Vector2 & point)
{
	if (index >= 3)
		return false;
	point = m_position;
	switch (m_direction)
	{
	case Direction2D::Down:
		point.y += 2;
	default:
	case Direction2D::Up:
		point.x += index;
		break;
	case Direction2D::Right:
		point.x += 2;
	case Direction2D::Left:
		point.y += index;
		break;
	}
	return true;
}

const std::string & Tank::getType() const { return m_isEnemy ? COLLIDER_TYPE_ENEMY_TANK : COLLIDER_TYPE_FRIEND_TANK; }

void Tank::DrawTank()
{
	//for (size_t x = 0; x < TANK_WIDTH; ++x)
	//	for (size_t y = 0; y < TANK_HEIGHT; ++y)
	//		CacheString(x, y, GetStringByState(x, y, m_direction));
	for (size_t y = 0; y < TANK_HEIGHT; ++y)
		CacheString(0, y, GetStringByState(y, m_direction, int(m_type)),
			{ m_isEnemy ? HealthColors[int(m_healthPoint/(float)m_maxHealth*_countof(HealthColors)) - 1] : m_color, DEFAULT_BACK_COLOR });
}

void Tank::Regerm()
{
	m_position = m_germPosition;
	setDirection(m_isEnemy ? Direction2D::Down : Direction2D::Up);
}
