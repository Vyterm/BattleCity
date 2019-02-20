#ifndef GAME_ENEMY_HPP_INCLUDED
#define GAME_ENEMY_HPP_INCLUDED

#include "TankController.hpp"

#include <ctime>

class Enemy : public TankController
{
protected:
	TankState IndirectDirection();
	TankState DirectDirection();
public:
	Enemy();
	virtual ~Enemy();
};

#endif