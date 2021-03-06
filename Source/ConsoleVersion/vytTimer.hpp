#ifndef VYT_TIMER_HPP
#define VYT_TIMER_HPP

#include "vytVector.hpp"

#include <map>
#include <functional>
#include <ctime>

namespace vyt
{
	// How to create a handler?
	// Step 1 : define a class (named like ticktock) public derived on game::timer::handler
	// Step 1.1 : The unit of parameter "invokeTime" is millseconds
	// Step 1.2 : Parameter "isLoop" is checked at Invoke, if it is false, will remove timer after invoke done.
	// Step 2 : Implement Invoke function. Function prototype: void Invoke();
	// Step 3 : Register ticktock to game::timer with params(call RegisterHandler), the sub params is params of the ticktock construction.
	// Step 4 : Loop call timer::handler::get_instance().HandleClock() to auto update timer.
	// Step 5 : Unregister ticktock like the following ways.
	// Step 5.1 : Save the return value, while want to unregister it, unregister it to game::timer(call UnregiserHandler).//Don't call it on Invoke()!
	// Step 5.2 : Call StopTimer will direct remove timer after invoke done.
	class timer
	{
	public:
		class handler
		{
		private:
			clock_t m_timer;
			clock_t m_invokeTime;
			bool m_isLoop;
			bool m_isAlive;
		protected:
			handler(clock_t invokeTime, bool isLoop) : m_invokeTime(invokeTime), m_isLoop(isLoop), m_timer(invokeTime), m_isAlive(true) { }
		public:
			virtual ~handler() { }
			virtual void Invoke() = NULL;
			bool Clock(clock_t timeDelta)
			{
				if (!m_isAlive) return true;
				m_timer -= timeDelta;
				if (m_timer > 0) return false;
				Invoke();
				if (!m_isLoop) return true;
				m_timer %= m_invokeTime;
				m_timer += m_invokeTime;
				return false;
			}
			void Reset(clock_t invokeTime)
			{
				m_invokeTime = invokeTime;
				m_timer = invokeTime;
			}
			void StopTimer()
			{
				m_isAlive = false;
			}
		};
		template <typename TItem>
		handler& RegisterHandler()
		{
			auto index = m_handlers.size();
			m_handlers.Append<TItem>();
			return m_handlers[index];
		}
		template <typename TItem, typename TArg1>
		handler& RegisterHandler(TArg1 &arg1)
		{
			auto index = m_handlers.size();
			m_handlers.Append<TItem>(arg1);
			return m_handlers[index];
		}
		template <typename TItem, typename TArg1, typename TArg2>
		handler& RegisterHandler(TArg1 &arg1, const TArg2 &arg2)
		{
			auto index = m_handlers.size();
			m_handlers.Append<TItem>(arg1, arg2);
			return m_handlers[index];
		}
		template <typename TItem, typename TArg1, typename TArg2, typename TArg3>
		handler& RegisterHandler(TArg1 &arg1, const TArg2 &arg2, const TArg3 &arg3)
		{
			auto index = m_handlers.size();
			m_handlers.Append<TItem>(arg1, arg2, arg3);
			return m_handlers[index];
		}
		void UnregiserHandler(handler &handler)
		{
			m_handlers.DeleteByReference(&handler);
		}
		void HandleClock()
		{
			auto newTimer = clock();
			auto timedelta = newTimer - m_timer;
			m_timer = newTimer;
			std::vector<handler*> deleteVector;
			for (auto &handler : m_handlers)
			{
				if (handler.Clock(timedelta))
					deleteVector.push_back(&handler);
			}
			for (auto &handler : deleteVector)
				m_handlers.DeleteByReference(handler);
		}
		static timer& get_instance()
		{
			if (nullptr == m_instance)
				m_instance = new timer();
			return *m_instance;
		}
	private:
		clock_t m_timer;
		vyt::vector<handler> m_handlers;
		static timer *m_instance;
		timer() {}
	};

	// How to use a auto timer?
	// Step 1 : Register timer with object pointer, per object only has one timer.
	// Step 1.1 : The unit of parameter "deltaTime" is millseconds
	// Step 2 : Implement Process function in that object class. Function prototype: void Process();
	// Step 3 : Unregister timer before object be destroy.
	template <typename T>
	class autoTimer : public vyt::timer::handler
	{
	private:
		T &m_object;
	public:
		autoTimer(T &object, clock_t deltaTime) : handler(deltaTime, true), m_object(object) { }
		void Invoke() { m_object.Process(); }
	private:
		static std::map<const T*, autoTimer*> m_staticTimers;
	public:
		static void RegisterTimer(T &object, clock_t deltaTime)
		{
			auto iter = m_staticTimers.find(&object);
			if (iter != m_staticTimers.end()) { iter->second->StopTimer(); }
			m_staticTimers[&object] = (autoTimer*)&vyt::timer::get_instance().RegisterHandler<autoTimer>(object, deltaTime);
		}
		static bool ChangeDeltaTime(const T &object, clock_t deltaTime)
		{
			auto iter = m_staticTimers.find(&object);
			if (iter == m_staticTimers.end()) return false;
			iter->second->Reset(deltaTime);
			return true;
		}
		static void UnregisterTimer(const T &object)
		{
			auto iter = m_staticTimers.find(&object);
			if (iter != m_staticTimers.end())
			{
				iter->second->StopTimer();
				m_staticTimers.erase(iter);
			}
		}
	};
	template <typename T>
	std::map<const T*, autoTimer<T>*> autoTimer<T>::m_staticTimers;

}

#endif