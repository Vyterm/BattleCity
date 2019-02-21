#include "GamePlayer.hpp"
#include "winapi.hpp"

const PlayerKeyModel Player::PlayerKeys[] = {
	{'W','A','S','D','F'},
	{ VK_UP, VK_LEFT,VK_DOWN,VK_RIGHT, VK_NUMPAD0 },
};

Player::Player(string name, const PlayerKeyModel & keys)
	: TankController(false), m_name(name), m_keys(keys)
{
}

Player::~Player() {}

bool Player::AttackTo(Tank & tank)
{
	bool isAttack = TankController::AttackTo(tank);
	if (isAttack && !tank.isAlive())
		m_score += (int(tank.getModel().type) + 1) * 10;
	return isAttack;
}

void Player::ActiveDraw()
{
	m_tank.Reset();
}

void Player::DeactiveDraw()
{
	m_tank.Clear();
}

TankState Player::IndirectDirection()
{
	Direction2D target = Direction2D::None;
	if (IsKeyDown(m_keys.kLeft))
		target = Direction2D::Left;
	if (IsKeyDown(m_keys.kUp))
		target = Direction2D::Up;
	if (IsKeyDown(m_keys.kRight))
		target = Direction2D::Right;
	if (IsKeyDown(m_keys.kDown))
		target = Direction2D::Down;
	bool isFire = IsKeyDown(m_keys.kFire);
	return { target, isFire };
}

TankState Player::DirectDirection()
{
	Direction2D target = Direction2D::None;
	if (IsKey(m_keys.kLeft))
		target = Direction2D::Left;
	if (IsKey(m_keys.kUp))
		target = Direction2D::Up;
	if (IsKey(m_keys.kRight))
		target = Direction2D::Right;
	if (IsKey(m_keys.kDown))
		target = Direction2D::Down;
	bool isFire = IsKey(m_keys.kFire);
	return { target, isFire };
}