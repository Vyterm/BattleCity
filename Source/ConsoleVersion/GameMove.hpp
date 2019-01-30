#ifndef GAME_MOVE_HPP_INCLUDED
#define GAME_MOVE_HPP_INCLUDED

#include "vytTimer.hpp"

namespace game
{
	class Controller
	{
	private:
		vyt::timer::handler *m_timer;
		int m_speedLevel;
		bool m_isActive;

		clock_t RealSpeed() const;
	public:
		clock_t TextSpeed() const;
		int get_Speed() const;
		void set_Speed(int speed);
		bool get_Active() const;
		void set_Active(bool isActive);
	public:
		Controller();
		virtual ~Controller();
		virtual void Process() = NULL;
	protected:
		virtual void OnEnable() {}
		virtual void OnDisable() {}
	};
}

//class TankMoveCtrl : public Controller
//{
//protected:
//	Tank m_tank;
//public:
//	TankMoveCtrl(E_TankType tankType, E_4BitColor color, bool isEnemy);
//	virtual ~TankMoveCtrl();
//	virtual E_Direction IndirectDirection() = NULL;
//	virtual E_Direction DirectDirection() = NULL;
//	virtual bool IsFire();
//	void Process();
//};
//
//class PlayerTankCtrl : public TankMoveCtrl
//{
//	int m_kUp, m_kLeft, m_kDown, m_kRight, m_kFire;
//public:
//	PlayerTankCtrl(E_TankType type, E_4BitColor color, bool isEnemy, int kUp, int kLeft, int kDown, int kRight, int kFire);
//	E_Direction IndirectDirection();
//	E_Direction DirectDirection();
//	bool IsFire();
//};
//
#endif