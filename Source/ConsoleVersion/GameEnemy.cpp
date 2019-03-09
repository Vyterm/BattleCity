#include "GameEnemy.hpp"

Enemy* Enemy::m_superEnemy = nullptr;

Direction2D Enemy::RandDirection() const
{
	switch (rand() % 4)
	{
	case 0: return Direction2D::Left;
	case 1: return Direction2D::Up;
	case 2: return Direction2D::Down;
	case 3: return Direction2D::Right;
	default: return Direction2D::None;
	}
}

TankState Enemy::IndirectDirection()
{
	if (0 == rand() % 8)
	{
		m_nextDirection = RandDirection();
		if (0 == rand() % 3)
			m_stopTimer += 2;
	}
	return TankState::Idle;
}

TankState Enemy::NormalDirectDirection()
{
	bool isFire = true;
	if (m_stopTimer > 0)
	{
		--m_stopTimer;
		isFire = false;
	}
	else if (!m_tank.Moveable())
	{
		if (0 == rand() % 4)
		{
			m_nextDirection = RandDirection();
			m_stopTimer += 2;
		}
		else
		{
			m_nextDirection = m_tank.getDirection().Clockwise();
			if (!m_tank.Moveable(m_nextDirection))
			{
				if (m_tank.Moveable(m_nextDirection.Reverse()))
					m_nextDirection = m_nextDirection.Reverse();
				else if (m_tank.Moveable(m_nextDirection.Reverse().Clockwise()))
					m_nextDirection = m_nextDirection.Reverse().Clockwise();
			}
		}
	}
	if (m_tank.getPosition().y == GAME_HEIGHT - 4)
		return { m_nextDirection = m_tank.getPosition().x < GAME_WIDTH / 2 ? Direction2D::Right : Direction2D::Left, true };
	TankState ts = { Direction2D::None == m_nextDirection ? m_tank.getDirection() : m_nextDirection, isFire };
	m_nextDirection = Direction2D::None;
	return ts;
}

TankState Enemy::SuperDirectDirection()
{
	if (m_lastAstar - clock() < 2000)
	{
		m_astar.InitPointInfo(getPosition(), { GAME_WIDTH / 2 - 1, 36 });
		if (m_astar.FindShortestPath())
			m_astar.GetShortestPath();
	}
	return TankState(m_astar.PopNextDirection(), true);
}

TankState Enemy::DirectDirection()
{
	return m_superEnemy == this ? SuperDirectDirection() : NormalDirectDirection();
}

Enemy::Enemy() : TankController(true)
{
}

Enemy::~Enemy()
{
	if (this == m_superEnemy)
		m_superEnemy = nullptr;
}

void Enemy::ApplyModel(const TankModel &model)
{
	TankController::ApplyModel(model);
	if (nullptr == m_superEnemy && model.type == E_TankType::Light)
		m_superEnemy = this;
}

void Enemy::Process()
{
	TankController::Process();
	if (this == m_superEnemy && !get_Active())
		m_superEnemy = nullptr;
}
