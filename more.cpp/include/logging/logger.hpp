#pragma once

#include <functional>

#include "logging\event.hpp"
#include "logging\formatter.hpp"

namespace logging
{
	// performs actual logging.
	class logger
	{
	public:

		formatter formatter;
		std::function<void(const event&)> write;
	};
}