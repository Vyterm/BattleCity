#include "GameTank.hpp"
#include "ColliderDefines.hpp"

constexpr size_t TANK_WIDTH = 3;
constexpr size_t TANK_HEIGHT = 3;
extern const int MIN_HEALTH;
extern const int MAX_HEALTH;

static const string TankParts[4][3][4] =
{
  {
	{"¨©§¨Ž", "¨©¥¨Ž", "¨©×¨Ž", "¨©×¨Ž"},
	{"©Ç¡ñ©Ï", "©Ç¡ñ©Ï", "©¥¡ñ©§", "©§¡ñ©¥"},
	{"¨©¥¨", "¨©§¨", "¨©ß¨", "¨©ß¨"}
  },
  {
	{"©³©§©·", "©³©×©·", "©³©×©·", "©³©×©·"},
	{"©Ç¡ñ©Ï", "©Ç¡ñ©Ï", "©¥¡ñ©Ï", "©Ç¡ñ©¥"},
	{"©»©ß©¿", "©»©§©¿", "©»©ß©¿", "©»©ß©¿"}
  },
  {
	{"©³©§©·", "¨©¥¨Ž", "©³©×¨Ž", "¨©×©·"},
	{"©Ç¡ñ©Ï", "©Ç¡ñ©Ï", "©¥¡ñ©§", "©§¡ñ©¥"},
	{"¨©¥¨", "©»©§©¿", "©»©ß¨", "¨©ß©¿"}
  },
  {
	{"¨X©§¨[", "¨X¨j¨[", "¨X¨j¨[", "¨X¨j¨["},
	{"¨d¨€¨g", "¨d¨€¨g", "©¥¨€¨g", "¨d¨€©¥"},
	{"¨^¨m¨a", "¨^©§¨a", "¨^¨m¨a", "¨^¨m¨a"}
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
	game::Renderer(TANK_WIDTH, TANK_HEIGHT, game::RenderType::DymanicLayer0),
	m_type(model.type), m_color(model.color), m_isEnemy(isEnemy),
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
	DrawTank();
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
	StrikeToActiveColliders();
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
