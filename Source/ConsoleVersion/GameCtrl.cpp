#include "GameCtrl.hpp"

#include <math.h>
#include <iostream>
using namespace std;

#pragma region Construct & Destruct
PlayerCtrl::PlayerCtrl(string name, GameMap &map, bool &isUpdateUI, int kUp, int kLeft, int kDown, int kRight)
	: m_name(name), m_map(map), m_isUpdateUI(isUpdateUI), m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight), m_speedLevel(0), m_score(0)
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

TankPlayerCtrl::TankPlayerCtrl(string name, GameMap & map, bool & isUpdateUI, E_4BitColor color, int kUp, int kLeft, int kDown, int kRight)
	: PlayerCtrl(name, map, isUpdateUI, kUp, kLeft, kDown, kRight), m_color(color)
{
}

void TankPlayerCtrl::Clear()
{
	PlayerCtrl::Clear();
}

void TankPlayerCtrl::Reset(Vector2 position)
{
	PlayerCtrl::Reset(position);
	m_position = position;
	DrawTank();
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

E_Direction PlayerCtrl::UpdateDirection()
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

void TankPlayerCtrl::ClearTank()
{
	m_map.Index(m_position.x, m_position.y) = E_CellType::None;
	m_map.Index(m_position.x - 1, m_position.y) = E_CellType::None;
	m_map.Index(m_position.x + 1, m_position.y) = E_CellType::None;
	m_map.Index(m_position.x, m_position.y - 1) = E_CellType::None;
	m_map.Index(m_position.x - 1, m_position.y - 1) = E_CellType::None;
	m_map.Index(m_position.x + 1, m_position.y - 1) = E_CellType::None;
	m_map.Index(m_position.x, m_position.y + 1) = E_CellType::None;
	m_map.Index(m_position.x - 1, m_position.y + 1) = E_CellType::None;
	m_map.Index(m_position.x + 1, m_position.y + 1) = E_CellType::None;
}

void TankPlayerCtrl::DrawTank()
{
	m_map.Index(m_position.x, m_position.y) = E_CellType::Tank;
	m_map.Index(m_position.x - 1, m_position.y) = E_CellType::Tank;
	m_map.Index(m_position.x + 1, m_position.y) = E_CellType::Tank;
	m_map.Index(m_position.x, m_position.y - 1) = E_CellType::Tank;
	m_map.Index(m_position.x - 1, m_position.y - 1) = E_CellType::Tank;
	m_map.Index(m_position.x + 1, m_position.y - 1) = E_CellType::Tank;
	m_map.Index(m_position.x, m_position.y + 1) = E_CellType::Tank;
	m_map.Index(m_position.x - 1, m_position.y + 1) = E_CellType::Tank;
	m_map.Index(m_position.x + 1, m_position.y + 1) = E_CellType::Tank;
}

#pragma endregion

void TankPlayerCtrl::Process()
{
	m_timer->Reset(clock_t(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel)));
	if (!m_alive) return;
	E_Direction target = PlayerCtrl::UpdateDirection();
	if (E_Direction::None == target) return;
	ClearTank();
	m_position = GetPositionByDirection(m_position, target);
	DrawTank();
}
