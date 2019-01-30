#ifndef GAME_ENEMY_HPP_INCLUDED
#define GAME_ENEMY_HPP_INCLUDED

#include "TankController.hpp"

#include <ctime>

class Enemy : public TankController
{
protected:
	TankState IndirectDirection()
	{
		auto ts = rand() % 6;
		switch (ts)
		{
		case 1: return TankState::Fire;
		case 2: return TankState::MoveLeft;
		case 3: return TankState::MoveUp;
		case 4: return TankState::MoveDown;
		case 5: return TankState::MoveRight;
		default: return TankState::Idle;
		}
	}
	TankState DirectDirection()
	{
		auto ts = rand() % 6;
		switch (ts)
		{
		case 1: return TankState::Fire;
		case 2: return TankState::MoveLeft;
		case 3: return TankState::MoveUp;
		case 4: return TankState::MoveDown;
		case 5: return TankState::MoveRight;
		default: return TankState::Idle;
		}
	}
public:
	Enemy(E_TankType type, E_4BitColor color) : TankController(type, color, true)
	{
		set_Speed(0);
	}
	virtual ~Enemy()
	{

	}
};

#endif