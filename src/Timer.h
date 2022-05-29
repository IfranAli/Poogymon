#ifndef LTIMER_H
#define LTIMER_H

#include <cstdint>
class LTimer {
	private:
		uint32_t mStartTicks;
		uint32_t mPausedTicks;
		bool mPaused;
		bool mStarted;
	public:
		LTimer();
		void start();
		void stop();
		void pause();
		void unpause();
		uint32_t getTicks();
		bool isStarted();
		bool isPaused();
};

#endif
