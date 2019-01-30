#include "GameController.hpp"
#include "GameDefines.hpp"

#include <cmath>

namespace game
{
	class ticktock : public vyt::timer::handler
	{
		Controller &m_player;
	public:
		ticktock(Controller &player, clock_t tickTime, bool isLoop) : vyt::timer::handler(tickTime, isLoop), m_player(player) { }
		void Invoke() { m_player.Process(); }
	};

	clock_t Controller::RealSpeed() const
	{
		return clock_t(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel));
	}

	clock_t Controller::TextSpeed() const
	{
		return clock_t(SPEED_DELTA / pow(ACCELERATING_FACTOR, m_speedLevel));
	}

	int Controller::get_Speed() const
	{
		return m_speedLevel;
	}

	void Controller::set_Speed(int speed)
	{
		m_speedLevel = speed;
		if (nullptr != m_timer)
		{
			if (RealSpeed() > 0)
				m_timer->Reset(RealSpeed());
			else
				m_timer->StopTimer();
		}
	}

	bool Controller::get_Active() const
	{
		return m_isActive;
	}

	void Controller::set_Active(bool isActive)
	{
		if (m_isActive == isActive) return;
		m_isActive = isActive;
		if (m_isActive)
		{
			m_timer = &vyt::timer::get_instance().RegisterHandler<ticktock>(*this, RealSpeed(), true);
			OnEnable();
		}
		else
		{
			// ToDo: timer unregister must be refactor, because if unregister on invoke will direct throw exception
			// While on process, timer can't direct remove, but can set isLoop=false to break out. To refactor unregisterTiemr fix it!
			//vyt::timer::get_instance().UnregiserHandler(*m_timer);
			m_timer->StopTimer();
			m_timer = nullptr;
			OnDisable();
		}
	}

	Controller::Controller() : m_isActive(false), m_timer(nullptr)
	{
		set_Active(true);
	}

	Controller::~Controller()
	{
		set_Active(false);
	}
}
