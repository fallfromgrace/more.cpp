#pragma once

#include "high_resolution_clock.hpp"

namespace chrono
{
	// 
	class stopwatch
	{
	public:
		// 
		stopwatch() : 
			start_time(high_resolution_clock::duration::zero()),
			stop_time(high_resolution_clock::duration::zero())
		{

		}

		// 
		inline void start()
		{
			this->start_time = high_resolution_clock::now().time_since_epoch();
		}

		// 
		inline void stop()
		{
			this->stop_time = high_resolution_clock::now().time_since_epoch();
		}

		// 
		inline void reset()
		{
			start_time = high_resolution_clock::duration::zero();
			stop_time = high_resolution_clock::duration::zero();
		}

		// 
		inline void restart()
		{
			reset();
			start();
		}

		// 
		inline high_resolution_clock::duration elapsed() const
		{
			return stop_time - start_time;
		}

	private:
		high_resolution_clock::duration start_time;
		high_resolution_clock::duration stop_time;
	};
}