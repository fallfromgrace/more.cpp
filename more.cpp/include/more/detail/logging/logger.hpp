#pragma once

#include <functional>

#include "more\detail\logging\event.hpp"
#include "more\detail\logging\formatter.hpp"

namespace more
{
	// performs actual logging.
	class logger
	{
	public:

		formatter formatter;
		std::function<void(const event&)> write;
	};
}