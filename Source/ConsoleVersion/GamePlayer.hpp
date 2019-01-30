#ifndef GAME_PLAYER_HPP_INCLUDED
#define GAME_PLAYER_HPP_INCLUDED

#include "GameCtrl.hpp"

class Player : public TankController
{
private:
	int m_kUp, m_kLeft, m_kDown, m_kRight, m_kFire;
protected:
	string m_name;
	int m_score;
public:
	string get_Name() const { return m_name; }

	void IncreaseScore() { ++m_score; }
	int get_Score() const { return m_score; }
protected:
	TankState IndirectDirection();
	TankState DirectDirection();

public:
	Player(string name, E_4BitColor color, int kUp, int kLeft, int kDown, int kRight, int kFire);
	virtual ~Player();
};

#endif