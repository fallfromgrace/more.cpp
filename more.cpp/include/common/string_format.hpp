#pragma once

#include <string>

namespace common
{
	// 
	template<typename... arg>
	std::string format(const std::string& format, arg&&... args)
	{
		char result[1024];
		sprintf_s(result, format.c_str(), std::forward<arg>(args)...);
		return std::string(result);
	}
}