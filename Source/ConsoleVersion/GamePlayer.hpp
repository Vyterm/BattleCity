#ifndef GAME_PLAYER_HPP_INCLUDED
#define GAME_PLAYER_HPP_INCLUDED

#include "GameDefines.hpp"
#include "TankController.hpp"

struct PlayerKeyModel
{
	int kUp, kLeft, kDown, kRight, kFire;
	PlayerKeyModel(int kUp, int kLeft, int kDown, int kRight, int kFire)
		: kUp(kUp), kLeft(kLeft), kDown(kDown), kRight(kRight), kFire(kFire) { }
};

class Player : public TankController
{
private:
	PlayerKeyModel m_keys;
protected:
	string m_name;
	int m_score;
public:
	string get_Name() const { return m_name; }

	void IncreaseScore() { ++m_score; }
	int get_Score() const { return m_score; }
	int get_LifeCount() const { return m_tank.getLifeCount(); }
	int get_Health() const { return m_tank.getHealth(); }
protected:
	TankState IndirectDirection();
	TankState DirectDirection();

public:
	Player(string name, const PlayerKeyModel &keyModel);
	virtual ~Player();
	virtual bool AttackTo(Tank &tank);
	void ActiveDraw();
	void DeactiveDraw();
public:
	static const PlayerKeyModel PlayerKeys[MAX_PLAYER_COUNT];
};

#endif