#include "GameTank.hpp"

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

Tank::Tank(E_TankType type) : game::Renderer(TANK_WIDTH, TANK_HEIGHT, game::RenderType::ActiveLayer0), m_type(type)
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
	m_position = target;
}

bool Tank::Contains(Vector2 position)
{
	return position.x - m_position.x < TANK_WIDTH && position.y - m_position.y < TANK_HEIGHT;
}

void Tank::DrawTank()
{
	//for (size_t x = 0; x < TANK_WIDTH; ++x)
	//	for (size_t y = 0; y < TANK_HEIGHT; ++y)
	//		CacheString(x, y, GetStringByState(x, y, m_direction));
	for (size_t y = 0; y < TANK_HEIGHT; ++y)
		CacheString(0, y, GetStringByState(y, m_direction, int(m_type)));
}
