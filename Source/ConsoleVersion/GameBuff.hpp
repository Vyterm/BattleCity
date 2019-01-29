#ifndef GAME_BUFF_HPP_INCLUDED
#define GAME_BUFF_HPP_INCLUDED

#include "GameColor.hpp"
#include "vytTimer.hpp"

class Player;

enum class E_BuffType
{
	Unstoppable,
	Incontrollable,
	Slippage,
};
static constexpr auto BuffCount = 3;


class PlayerBuff : public vyt::timer::handler
{
	int m_clockSecond;
	int m_tickCount;
	E_4BitColor m_playerColor;
	E_BuffType m_type;
protected:
	Player &m_player;
	bool m_isAppend;
	PlayerBuff(Player &player, int clockSecond, E_BuffType type);
	virtual ~PlayerBuff();
	virtual E_4BitColor ShiningColour() const { return m_playerColor; }
	virtual void Invoke();
public:
	virtual void Copy(const PlayerBuff *buff);
	int RemainSecond() const;
	virtual void RemoveBuff();
};


class UnstoppableBuff : public PlayerBuff
{
public:
	UnstoppableBuff(Player &player, int clockSecond);
	E_4BitColor ShiningColour() const { return E_4BitColor::LYellow; }
	virtual void RemoveBuff() override;
};
class IncontrollableBuff : public PlayerBuff
{
	int m_kUp, m_kLeft, m_kDown, m_kRight;
public:
	IncontrollableBuff(Player &player, int clockSecond);
	E_4BitColor ShiningColour() const { return E_4BitColor::Green; }
	virtual void RemoveBuff() override;
};
class SlippageBuff : public PlayerBuff
{
public:
	SlippageBuff(Player &player, int clockSecond);
	virtual void RemoveBuff() override;
};

#endif