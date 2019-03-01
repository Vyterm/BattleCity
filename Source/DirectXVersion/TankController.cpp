#include "TankController.hpp"
#include "GameBullet.hpp"

#include <math.h>
#include <iostream>
using namespace std;

TankController::TankController(bool isEnemy)
	: m_tank(isEnemy)
{
	m_fireDelta = isEnemy ? 1000 : 500;
	m_fireTimer = clock() - m_fireDelta;
}

void TankController::OnDisable()
{
	m_tank.Clear();
}

void TankController::OnEnable()
{
	m_tank.Reset();
}

void TankController::Process()
{
	if (!m_tank.isAlive()) set_Active(false);
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
	{
		auto now = clock();
		if (m_fireTimer + m_fireDelta > now) return;
		m_fireTimer = now;
		Bullet::Create(E_BulletType::Normal, m_tank.getPosition() + Vector2(1, 1)
			+ m_tank.getDirection() + m_tank.getDirection(), m_tank.getDirection(), *this);
	}
}

bool TankController::AttackTo(Tank &tank)
{
	if (m_tank.isEnemy() == tank.isEnemy()) return false;
	tank.ReduceHealth(getModel().attack);
	return true;
}

void TankController::ApplyModel(const TankModel &model)
{
	m_tank.ApplyModel(model);
	set_Speed(model.speed);
}
