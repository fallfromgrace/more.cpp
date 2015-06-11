#pragma once

#include <chrono>
#include <string>

#include "includes.hpp"

#include "logging\level.hpp"

namespace logging
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