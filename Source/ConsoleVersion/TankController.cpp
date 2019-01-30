#include "TankController.hpp"
#include "GameBullet.hpp"

#include <math.h>
#include <iostream>
using namespace std;

TankController::TankController(E_TankType type, E_4BitColor color, bool isEnemy)
	: m_tank(type, color, isEnemy)
{
}

void TankController::OnDisable()
{
}

void TankController::OnEnable()
{
	m_tank.Reset(m_germPosition);
}

void TankController::Process()
{
	if (!get_Active()) return;
	TankState direction = IndirectDirection();
	TankState target = DirectDirection();
	if (Direction2D::None != Direction2D(target))
	{
		m_tank.setDirection(target);
		auto targetPosition = m_tank.getPosition() + m_tank.getDirection();
		m_tank.Move(targetPosition);
	}
	else if (Direction2D::None != Direction2D(direction))
	{
		m_tank.setDirection(direction);
	}
	if (direction.isFire() || target.isFire())
		Bullet::Create(E_BulletType::Normal, 1, m_tank.getPosition() + Vector2(1, 1)
			+ m_tank.getDirection() + m_tank.getDirection(), m_tank.getDirection());
}
