#include "GameEnemy.hpp"

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
	if (m_tank.getPosition().y == GAME_HEIGHT - 4)
		return { m_nextDirection = m_tank.getPosition().x < GAME_WIDTH / 2 ? Direction2D::Right : Direction2D::Left, true };
	return TankState::Idle;
}

TankState Enemy::DirectDirection()
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
	TankState ts = { Direction2D::None == m_nextDirection ? m_tank.getDirection() : m_nextDirection, isFire };
	m_nextDirection = Direction2D::None;
	return ts;
}

Enemy::Enemy() : TankController(true)
{
}

Enemy::~Enemy()
{

}
