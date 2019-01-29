#include "GameCtrl.hpp"

#include <math.h>
#include <iostream>
using namespace std;

#pragma region Construct & Destruct
PlayerCtrl::PlayerCtrl(string name, bool &isUpdateUI, int kUp, int kLeft, int kDown, int kRight)
	: m_name(name), m_isUpdateUI(isUpdateUI), m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight), m_speedLevel(0), m_score(0)
{

}

PlayerCtrl::~PlayerCtrl() {}

void PlayerCtrl::Clear()
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

void PlayerCtrl::Reset(Vector2 position)
{
	m_timer = &(vyt::timer::get_instance().RegisterHandler<ticktock>(*this, 100, true));
	m_alive = true;
}

TankPlayerCtrl::TankPlayerCtrl(string name, bool & isUpdateUI, E_4BitColor color, int kUp, int kLeft, int kDown, int kRight)
	: PlayerCtrl(name, isUpdateUI, kUp, kLeft, kDown, kRight), m_color(color), m_tank(E_TankType::Assault, color, false)
{
}

void TankPlayerCtrl::Clear()
{
	PlayerCtrl::Clear();
}

void TankPlayerCtrl::Reset(Vector2 position)
{
	PlayerCtrl::Reset(position);
	m_tank.Reset(position);
}

#pragma endregion

#pragma region Properties

void PlayerCtrl::get_keyCtrl(int &kUp, int &kLeft, int &kDown, int &kRight)
{
	kUp = m_kUp;
	kLeft = m_kLeft;
	kDown = m_kDown;
	kRight = m_kRight;
}
void PlayerCtrl::set_keyCtrl(int kUp, int kLeft, int kDown, int kRight)
{
	m_kUp = kUp;
	m_kLeft = kLeft;
	m_kDown = kDown;
	m_kRight = kRight;
}

#pragma endregion

#pragma region Process Methods

E_Direction PlayerCtrl::UndeterminedDirection()
{
	E_Direction target;
	if (IsKeyDown(m_kLeft))
		target = E_Direction::Left;
	else if (IsKeyDown(m_kUp))
		target = E_Direction::Up;
	else if (IsKeyDown(m_kRight))
		target = E_Direction::Right;
	else if (IsKeyDown(m_kDown))
		target = E_Direction::Down;
	else
		target = E_Direction::None;
	return target;
}

E_Direction PlayerCtrl::CurrentDirection()
{
	E_Direction target;
	if (IsKey(m_kLeft))
		target = E_Direction::Left;
	else if (IsKey(m_kUp))
		target = E_Direction::Up;
	else if (IsKey(m_kRight))
		target = E_Direction::Right;
	else if (IsKey(m_kDown))
		target = E_Direction::Down;
	else
		target = E_Direction::None;
	return target;
}

#pragma endregion

#pragma region Tank Process Methods

#pragma endregion

void TankPlayerCtrl::Process()
{
	m_timer->Reset(clock_t(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel)));
	if (!m_alive) return;
	E_Direction direction = UndeterminedDirection();
	E_Direction target = PlayerCtrl::CurrentDirection();
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
}
