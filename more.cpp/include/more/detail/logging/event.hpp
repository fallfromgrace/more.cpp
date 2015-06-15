#pragma once

#include <chrono>
#include <string>

#include "more\includes.hpp"
#include "more\detail\logging\level.hpp"

namespace more
{
	// 
	class event
	{
	public:
		event()
		{

		}

		event(int32_t level)
		{

		}

		int32_t level;
		uint64_t thread_id;
		std::chrono::system_clock::time_point timestamp;
		std::string message;
	};
}