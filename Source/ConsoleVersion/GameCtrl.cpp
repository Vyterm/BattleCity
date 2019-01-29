#include "GameCtrl.hpp"
#include "GameBullet.hpp"

#include <math.h>
#include <iostream>
using namespace std;

#pragma region Construct & Destruct
Player::Player(string name, bool &isUpdateUI, int kUp, int kLeft, int kDown, int kRight)
	: m_name(name), m_isUpdateUI(isUpdateUI), m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight), m_speedLevel(0), m_score(0)
{

}

Player::~Player() {}

void Player::Clear()
{
	vyt::timer::get_instance().UnregiserHandler(*m_timer);
	for (int i = 0; i < BuffCount; ++i)
		if (nullptr != m_buffs[i])
		{
			m_buffs[i]->RemoveBuff();
			m_buffs[i] = nullptr;
		}
	m_alive = false;
}

void Player::Reset()
{
	m_timer = &(vyt::timer::get_instance().RegisterHandler<ticktock>(*this, 100, true));
	m_alive = true;
}

TankPlayerCtrl::TankPlayerCtrl(string name, bool & isUpdateUI, E_4BitColor color, int kUp, int kLeft, int kDown, int kRight)
	: Player(name, isUpdateUI, kUp, kLeft, kDown, kRight), m_color(color), m_tank(E_TankType::Assault, color, false)
{
}

void TankPlayerCtrl::Clear()
{
	Player::Clear();
}

void TankPlayerCtrl::Reset()
{
	Player::Reset();
	m_tank.Reset(m_germPosition);
}

#pragma endregion

#pragma region Properties

void Player::get_keyCtrl(int &kUp, int &kLeft, int &kDown, int &kRight)
{
	kUp = m_kUp;
	kLeft = m_kLeft;
	kDown = m_kDown;
	kRight = m_kRight;
}
void Player::set_keyCtrl(int kUp, int kLeft, int kDown, int kRight)
{
	m_kUp = kUp;
	m_kLeft = kLeft;
	m_kDown = kDown;
	m_kRight = kRight;
}

#pragma endregion

#pragma region Process Methods

E_Direction Player::IndirectDirection()
{
	E_Direction target = E_Direction::None;
	if (IsKeyDown(m_kLeft))
		target = E_Direction::Left;
	if (IsKeyDown(m_kUp))
		target = E_Direction::Up;
	if (IsKeyDown(m_kRight))
		target = E_Direction::Right;
	if (IsKeyDown(m_kDown))
		target = E_Direction::Down;
	return target;
}

E_Direction Player::DirectDirection()
{
	if (IsKey(m_kLeft))
		return E_Direction::Left;
	else if (IsKey(m_kUp))
		return E_Direction::Up;
	else if (IsKey(m_kRight))
		return E_Direction::Right;
	else if (IsKey(m_kDown))
		return E_Direction::Down;
	else
		return E_Direction::None;
}

#pragma endregion

#pragma region Tank Process Methods

#pragma endregion

void TankPlayerCtrl::Process()
{
	m_timer->Reset(clock_t(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel)));
	if (!m_alive) return;
	E_Direction direction = IndirectDirection();
	E_Direction target = Player::DirectDirection();
	if (E_Direction::None != target)
	{
		m_tank.setDirection(target);
		auto targetPosition = GetPositionByDirection(m_tank.getPosition(), m_tank.getDirection());
		m_tank.Move(targetPosition);
	}
	else if (E_Direction::None != direction)
	{
		m_tank.setDirection(direction);
	}
	auto kFire = m_name == "Íæ¼ÒÒ»" ? 'F' : VK_NUMPAD0;
	if (IsKeyDown(kFire))
		Bullet::Create(E_BulletType::Normal, 1, m_tank.getPosition() + Vector2(1, 1)
			+ GetDirectionVector(m_tank.getDirection()) + GetDirectionVector(m_tank.getDirection()), m_tank.getDirection());
}
