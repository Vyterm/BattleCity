#ifndef GAME_FPS_HPP_INCLUDED
#define GAME_FPS_HPP_INCLUDED
#include "GameDefines.hpp"
#include "winapi.hpp"

#include <sstream>
#include <ctime>

namespace game
{
	class FPS
	{
		int fps;
		clock_t lastFrame;
		clock_t newFrame;
	public:
		FPS() : fps(0)
		{
			lastFrame = clock();
			newFrame = clock();
		}
		~FPS()
		{
			SetTitle(GAME_NAME);
		}
		void Frame()
		{
			++fps;
			newFrame = clock();
			if (newFrame - lastFrame > 1000)
			{
				std::ostringstream ss;
				ss << GAME_NAME << "  (FPS:" << fps << ")";
				SetTitle(ss.str().c_str());
				fps = 0;
				lastFrame = clock();
			}
		}
	};
}

#endif
