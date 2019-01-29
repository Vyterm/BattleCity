#include "GameMove.hpp"

#include <cmath>

constexpr auto SPEED_DELTA = 30;
constexpr auto ACCELERATING_FACTOR = 0.998;

class ticktock : public vyt::timer::handler
{
	MoveCtrl &m_player;
public:
	ticktock(MoveCtrl &player, clock_t tickTime, bool isLoop) : vyt::timer::handler(tickTime, isLoop), m_player(player) { }
	void Invoke() { m_player.Process(); }
};

clock_t MoveCtrl::RealSpeed() const
{
	return clock_t(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel));
}

clock_t MoveCtrl::TextSpeed() const
{
	return clock_t(SPEED_DELTA / pow(ACCELERATING_FACTOR, m_speedLevel));
}

int MoveCtrl::get_Speed() const
{
	return m_speedLevel;
}

void MoveCtrl::set_Speed(int speed)
{
	m_speedLevel = speed;
	if (nullptr != m_timer)
	{
		if (speed > 0)
			m_timer->Reset(RealSpeed());
		else
			m_timer->StopTimer();
	}
}

bool MoveCtrl::get_Active() const
{
	return m_isActive;
}

void MoveCtrl::set_Active(bool isActive)
{
	if (m_isActive == isActive) return;
	m_isActive = isActive;
	if (m_isActive)
	{
		m_timer = &vyt::timer::get_instance().RegisterHandler<ticktock>(*this, RealSpeed(), true);
		OnEnable();
	}
	else
	{
		// ToDo: timer unregister must be refactor, because if unregister on invoke will direct throw exception
		// While on process, timer can't direct remove, but can set isLoop=false to break out. To refactor unregisterTiemr fix it!
		//vyt::timer::get_instance().UnregiserHandler(*m_timer);
		m_timer->StopTimer();
		m_timer = nullptr;
		OnDisable();
	}
}

MoveCtrl::MoveCtrl() : m_isActive(false), m_timer(nullptr)
{
	set_Active(true);
}

MoveCtrl::~MoveCtrl()
{
	set_Active(false);
}

//TankMoveCtrl::TankMoveCtrl(E_TankType tankType, E_4BitColor color, bool isEnemy)
//	: m_tank(tankType, color, isEnemy)
//{
//}
//
//TankMoveCtrl::~TankMoveCtrl()
//{
//}
//
//void TankMoveCtrl::Process()
//{
//	E_Direction direction = IndirectDirection();
//	E_Direction target = DirectDirection();
//	if (E_Direction::None != target)
//	{
//		m_tank.setDirection(target);
//		auto targetPosition = GetPositionByDirection(m_tank.getPosition(), m_tank.getDirection());
//		m_tank.Move(targetPosition);
//	}
//	else if (E_Direction::None != direction)
//	{
//		m_tank.setDirection(direction);
//	}
//}
//
//PlayerTankCtrl::PlayerTankCtrl(E_TankType type, E_4BitColor color, bool isEnemy, int kUp, int kLeft, int kDown, int kRight, int kFire)
//	: TankMoveCtrl(type, color, isEnemy), m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight)
//{
//}
//
//E_Direction PlayerTankCtrl::IndirectDirection()
//{
//	return E_Direction();
//}
//
//E_Direction PlayerTankCtrl::DirectDirection()
//{
//	return E_Direction();
//}
//
//bool PlayerTankCtrl::IsFire()
//{
//	return false;
//}
