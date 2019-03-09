#ifndef GAME_ENEMY_HPP_INCLUDED
#define GAME_ENEMY_HPP_INCLUDED

#include "TankController.hpp"
#include "AStar.hpp"

#include <ctime>

class Enemy : public TankController
{
private:
	static Enemy* m_superEnemy;
	size_t m_stopTimer;
	Direction2D m_nextDirection;
	AStar m_astar;
	clock_t m_lastAstar;
private:
	Direction2D RandDirection() const;
	TankState NormalDirectDirection();
	TankState SuperDirectDirection();
protected:
	TankState IndirectDirection() override;
	TankState DirectDirection() override;
public:
	Enemy();
	virtual ~Enemy();
	void ApplyModel(const TankModel &model) override;
	void Process() override;
};

#endif