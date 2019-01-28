#ifndef GAME_CTRL_HPP
#define GAME_CTRL_HPP

#include "GameModel.hpp"
#include "GameBuff.hpp"
#include "GameTank.hpp"
#include "vytTimer.hpp"

#include <string>
#include <vector>
#include <math.h>

using std::string;

constexpr auto SPEED_DELTA = 30;
constexpr auto ACCELERATING_FACTOR = 0.998;

class PlayerCtrl
{
private:
	int m_kUp, m_kLeft, m_kDown, m_kRight;
protected:
	string m_name;
	bool m_alive = true, m_unstoppable = false;
	bool &m_isUpdateUI;
	int m_speedLevel, m_score;;
public:
	string get_Name() const { return m_name; }

	virtual bool get_Alive() const { return m_alive; }
	virtual void set_Alive(bool alive) { m_alive = alive; }

	virtual bool get_Unstoppable() const { return m_unstoppable; }
	virtual void set_Unstoppable(bool unstoppable) { m_unstoppable = unstoppable; }

	void get_keyCtrl(int &kUp, int &kLeft, int &kDown, int &kRight);
	void set_keyCtrl(int kUp, int kLeft, int kDown, int kRight);

	void IncreaseScore() { ++m_score; }
	int get_Score() const { return m_score; }
	void set_Speed(int speed) { m_speedLevel = speed; }
	int get_Speed() const { return int(SPEED_DELTA / pow(ACCELERATING_FACTOR, m_speedLevel)); }
	virtual void set_Color(const E_4BitColor &color) = NULL;
	virtual E_4BitColor get_Color() const = NULL;
	virtual void SetEnemy(PlayerCtrl &enemy) = NULL;
protected:
	E_Direction UndeterminedDirection();
	E_Direction CurrentDirection();
	vyt::timer::handler *m_timer;

	class ticktock : public vyt::timer::handler
	{
		PlayerCtrl &m_player;
	public:
		ticktock(PlayerCtrl &player, clock_t tickTime, bool isLoop) : vyt::timer::handler(tickTime, isLoop), m_player(player) { }
		void Invoke() { m_player.Process(); }
	};
	virtual void Process() = NULL;
private:
	PlayerBuff *m_buffs[BuffCount];
public:
	int get_BuffRemainSecond(E_BuffType type) const { return m_buffs[int(type)] == nullptr ? 0 : m_buffs[int(type)]->RemainSecond(); }
	friend PlayerBuff;

public:
	PlayerCtrl(string name, bool &isUpdateUI, int kUp, int kLeft, int kDown, int kRight);
	virtual ~PlayerCtrl();

	virtual void Clear();
	virtual void Reset(Vector2 position);
};

class TankPlayerCtrl : public PlayerCtrl
{
private:
	Tank m_tank;
	E_4BitColor m_color;
public:
	virtual void set_Color(const E_4BitColor &color) { m_color = color; }
	virtual E_4BitColor get_Color() const { return m_color; }
	virtual void SetEnemy(PlayerCtrl &enemy) { }
public:
	TankPlayerCtrl(string name, bool &isUpdateUI, E_4BitColor color, int kUp, int kLeft, int kDown, int kRight);

	void Clear();
	void Reset(Vector2 position);

	void Process();
};

#endif