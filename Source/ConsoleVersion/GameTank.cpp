#include "GameTank.hpp"
#include "CtrlDefines.hpp"

constexpr size_t TANK_WIDTH = 3;
constexpr size_t TANK_HEIGHT = 3;
static const string TankParts[4][3][4] =
{
  {
	{"������", "������", "���ר�", "���ר�"},
	{"�ǡ��", "�ǡ��", "����", "����"},
	{"������", "������", "���ߨ�", "���ߨ�"}
  },
  {
	{"������", "���ש�", "���ש�", "���ש�"},
	{"�ǡ��", "�ǡ��", "�����", "�ǡ�"},
	{"���ߩ�", "������", "���ߩ�", "���ߩ�"}
  },
  {
	{"������", "������", "���ר�", "���ש�"},
	{"�ǡ��", "�ǡ��", "����", "����"},
	{"������", "������", "���ߨ�", "���ߩ�"}
  },
  {
	{"�X���[", "�X�j�[", "�X�j�[", "�X�j�["},
	{"�d���g", "�d���g", "�����g", "�d����"},
	{"�^�m�a", "�^���a", "�^�m�a", "�^�m�a"}
  }
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

Tank::Tank(E_TankType type, E_4BitColor color, bool isEnemy) :
	game::Renderer(TANK_WIDTH, TANK_HEIGHT, game::RenderType::ActiveLayer0),
	game::Collider(false), m_type(type), m_isEnemy(isEnemy), m_direction(Direction2D::Up)
{
	SetDrawActive(false);
}

Tank::~Tank()
{
}

void Tank::Reset(Vector2 position)
{
	m_position = position;
	SetDrawActive(true);
	setColliderActive(true);
	DrawTank();
}

void Tank::Clear()
{
	SetDrawActive(false);
	setColliderActive(false);
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
		collider.getType() == COLLIDER_TYPE_FRIEND_TANK)
		m_moveable = false;
}

bool Tank::Contains(const Vector2 & position)
{
	return position.x - m_position.x < TANK_WIDTH && position.y - m_position.y < TANK_HEIGHT;
}

bool Tank::SetPositionByIndex(size_t index, Vector2 & point)
{
	if (index >= 9)
		return false;
	point = m_position;
	point.x += index % 3;
	point.y += index / 3;
	return true;
}

const std::string & Tank::getType() const { return m_isEnemy ? COLLIDER_TYPE_ENEMY_TANK : COLLIDER_TYPE_FRIEND_TANK; }

void Tank::DrawTank()
{
	//for (size_t x = 0; x < TANK_WIDTH; ++x)
	//	for (size_t y = 0; y < TANK_HEIGHT; ++y)
	//		CacheString(x, y, GetStringByState(x, y, m_direction));
	for (size_t y = 0; y < TANK_HEIGHT; ++y)
		CacheString(0, y, GetStringByState(y, m_direction, int(m_type)));
}
