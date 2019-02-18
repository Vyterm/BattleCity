#include "GamePlayer.hpp"

Player::Player(string name, E_4BitColor color, int kUp, int kLeft, int kDown, int kRight, int kFire)
	: TankController({ E_TankType::Assault, 3, color }, false), m_name(name), m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight), m_kFire(kFire), m_score(0)
{
	set_Speed(0);
}

Player::~Player() {}

bool Player::AttackTo(Tank & tank)
{
	bool isAttack = TankController::AttackTo(tank);
	if (isAttack && !tank.isAlive())
		m_score += (int(tank.getTankType()) + 1) * 10;
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
	if (IsKeyDown(m_kLeft))
		target = Direction2D::Left;
	if (IsKeyDown(m_kUp))
		target = Direction2D::Up;
	if (IsKeyDown(m_kRight))
		target = Direction2D::Right;
	if (IsKeyDown(m_kDown))
		target = Direction2D::Down;
	bool isFire = false;
	if (IsKeyDown(m_kFire))
		isFire = true;
	return { target, isFire };
}

TankState Player::DirectDirection()
{
	Direction2D target = Direction2D::None;
	if (IsKey(m_kLeft))
		target = Direction2D::Left;
	if (IsKey(m_kUp))
		target = Direction2D::Up;
	if (IsKey(m_kRight))
		target = Direction2D::Right;
	if (IsKey(m_kDown))
		target = Direction2D::Down;
	bool isFire = false;
	if (IsKey(m_kFire))
		isFire = true;
	return { target, isFire };
}