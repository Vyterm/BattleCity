#ifndef GAME_ENEMY_HPP_INCLUDED
#define GAME_ENEMY_HPP_INCLUDED

#include "TankController.hpp"

#include <ctime>

class Enemy : public TankController
{
private:
	size_t m_stopTimer;
	Direction2D m_nextDirection;
private:
	Direction2D RandDirection() const;
protected:
	TankState IndirectDirection() override;
	TankState DirectDirection() override;
public:
	Enemy();
	virtual ~Enemy();
};

#endif