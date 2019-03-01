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

#endif