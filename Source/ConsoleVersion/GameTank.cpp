#include "GameTank.hpp"
#include "CtrlDefines.hpp"

constexpr size_t TANK_WIDTH = 3;
constexpr size_t TANK_HEIGHT = 3;
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

inline const string& GetStringByState(size_t y, E_Direction direction, size_t modelType = 0)
{
	auto directionIndex = 0;
	switch (direction)
	{
	case E_Direction::Left: directionIndex = 2; break;
	case E_Direction::Right: directionIndex = 3; break;
	case E_Direction::Up: directionIndex = 0; break;
	case E_Direction::Down: directionIndex = 1; break;
	case E_Direction::None:
	default: break;
	}
	return TankParts[modelType][y][directionIndex];
}

Tank::Tank(E_TankType type, E_4BitColor color, bool isEnemy) :
	game::Renderer(TANK_WIDTH, TANK_HEIGHT, game::RenderType::ActiveLayer0),
	game::Collider(isEnemy ? COLLIDER_TYPE_ENEMY_TANK : COLLIDER_TYPE_FRIEND_TANK, true), m_type(type)
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
	DrawTank();
}

void Tank::Clear()
{
	SetDrawActive(false);
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
	if (collider.getType() == COLLIDER_TYPE_JEBEL_LANDSPACE || collider.getType() == COLLIDER_TYPE_ENEMY_TANK)
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

void Tank::DrawTank()
{
	//for (size_t x = 0; x < TANK_WIDTH; ++x)
	//	for (size_t y = 0; y < TANK_HEIGHT; ++y)
	//		CacheString(x, y, GetStringByState(x, y, m_direction));
	for (size_t y = 0; y < TANK_HEIGHT; ++y)
		CacheString(0, y, GetStringByState(y, m_direction, int(m_type)));
}
