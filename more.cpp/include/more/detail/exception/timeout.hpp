#pragma once

#include <stdexcept>

namespace more
{

	class timeout : public std::runtime_error
	{
	public:
		timeout() :
			std::runtime_error("The operation timed out.")
		{

		}
	};
}